#!/usr/bin/env python3
"""Bounded serial monitor and classifier for the ESP32 controller experiment."""

from __future__ import annotations

import argparse
import json
import signal
import subprocess
import sys
import time
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Iterable, TextIO

try:
    import serial
    from serial import SerialException
except ImportError:  # pragma: no cover - depends on host setup
    serial = None

    class SerialException(Exception):
        """Fallback used only to produce a useful dependency error."""


EXPECTED_ENVIRONMENT = "esp32doit-devkit-v1"
EXPECTED_BOARD = "esp32doit-devkit-v1"
EXPECTED_FIRMWARE = "esp32-bluetooth-controller-baseline"
EXPECTED_PROTOCOL_VERSION = 1
SERIAL_SPEED = 115200
BTDIAG_PREFIX = "BTDIAG "
INPUT_EVENTS = {"controller_first_input", "controller_input"}

SCRIPT_PATH = Path(__file__).resolve()
REPOSITORY_ROOT = SCRIPT_PATH.parents[4]
EXPECTED_PROJECT = REPOSITORY_ROOT / "experiments" / EXPECTED_FIRMWARE
DIAGNOSTICS_DIR = EXPECTED_PROJECT / ".diagnostics"


@dataclass
class Evidence:
    records: list[dict[str, Any]] = field(default_factory=list)
    malformed_btdiag_lines: int = 0
    unsupported_protocol_lines: int = 0
    firmware_mismatch_lines: int = 0

    def events(self) -> list[str]:
        return [str(record["event"]) for record in self.records]


@dataclass(frozen=True)
class Classification:
    name: str
    furthest_stage: str
    explanation: str


def parse_btdiag_line(line: str, evidence: Evidence) -> dict[str, Any] | None:
    marker = line.find(BTDIAG_PREFIX)
    if marker < 0:
        return None

    payload = line[marker + len(BTDIAG_PREFIX) :].strip()
    try:
        record = json.loads(payload)
    except json.JSONDecodeError:
        evidence.malformed_btdiag_lines += 1
        return None

    if not isinstance(record, dict):
        evidence.malformed_btdiag_lines += 1
        return None
    if not isinstance(record.get("event"), str) or not record["event"]:
        evidence.malformed_btdiag_lines += 1
        return None
    if not isinstance(record.get("elapsed_ms"), int):
        evidence.malformed_btdiag_lines += 1
        return None
    if record.get("protocol_version") != EXPECTED_PROTOCOL_VERSION:
        evidence.unsupported_protocol_lines += 1
        return None
    if record["event"] == "firmware_started" and record.get("firmware") != EXPECTED_FIRMWARE:
        evidence.firmware_mismatch_lines += 1
        return None

    evidence.records.append(record)
    return record


def has_requested_input(records: Iterable[dict[str, Any]], required_button: str) -> bool:
    connected = False
    for record in records:
        event = record["event"]
        if event == "controller_connected":
            connected = True
            continue
        if not connected or event not in INPUT_EVENTS or record.get("valid_report") is not True:
            continue
        buttons = {part.strip() for part in str(record.get("pressed_buttons", "")).split(",")}
        if not required_button or required_button in buttons:
            return True
    return False


def classify(evidence: Evidence, serial_opened: bool, required_button: str = "cross") -> Classification:
    if not serial_opened:
        return Classification(
            "serial_unavailable",
            "serial unavailable",
            "The selected serial device could not be validated or opened; no firmware conclusion is possible.",
        )

    events = evidence.events()
    if "firmware_started" not in events:
        detail = "Structured boot evidence from this experiment was not observed."
        if evidence.firmware_mismatch_lines:
            detail = "A BTDIAG boot record was observed, but it did not identify this experiment."
        return Classification("missing_experiment_boot", "serial open", detail)

    if "bluetooth_ready" not in events:
        return Classification(
            "bluetooth_initialization_incomplete",
            "firmware started",
            "The experiment booted, but Bluetooth readiness was not evidenced.",
        )
    if "accepting_connections" not in events:
        return Classification(
            "bluetooth_initialization_incomplete",
            "Bluetooth ready",
            "Bluetooth readiness was observed, but the connection window was not evidenced.",
        )

    if has_requested_input(evidence.records, required_button):
        return Classification(
            "successful_input",
            "requested controller input received",
            f"A controller connected and a subsequent valid input included {required_button or 'controller data'}.",
        )

    connected = "controller_connected" in events
    disconnected = "controller_disconnected" in events
    if connected and disconnected:
        return Classification(
            "premature_disconnection",
            "controller disconnected before requested input",
            "A controller connected but disconnected before the requested valid input was evidenced.",
        )
    if connected:
        return Classification(
            "connected_without_input",
            "controller connected",
            "A controller connection was observed, but the requested valid input was not.",
        )
    if "connection_timeout" in events:
        return Classification(
            "pairing_timeout",
            "connection window timed out",
            "No usable controller connection was observed within the firmware's bounded window.",
        )
    return Classification(
        "waiting_for_controller",
        "accepting connections",
        "The ESP32 reached its connection window, but no controller connection was evidenced before monitoring ended.",
    )


def read_project_setting(project: Path, key: str) -> str | None:
    prefix = f"{key} ="
    for raw_line in (project / "platformio.ini").read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if line.startswith(prefix):
            return line.split("=", 1)[1].strip()
    return None


def validate_project(project: Path, environment: str) -> None:
    if project.resolve() != EXPECTED_PROJECT.resolve():
        raise ValueError(f"project must be {EXPECTED_PROJECT}")
    required = [project / "platformio.ini", project / "scripts" / "pio.sh", project / "main" / "btdiag.cpp"]
    missing = [str(path) for path in required if not path.is_file()]
    if missing:
        raise ValueError(f"experiment is incomplete; missing: {', '.join(missing)}")
    ini_text = (project / "platformio.ini").read_text(encoding="utf-8")
    if f"[env:{environment}]" not in ini_text:
        raise ValueError(f"PlatformIO environment {environment!r} is not defined")
    if read_project_setting(project, "default_envs") != EXPECTED_ENVIRONMENT:
        raise ValueError(f"default PlatformIO environment is not {EXPECTED_ENVIRONMENT}")
    if read_project_setting(project, "board") != EXPECTED_BOARD:
        raise ValueError(f"configured board is not {EXPECTED_BOARD}")
    if read_project_setting(project, "monitor_speed") != str(SERIAL_SPEED):
        raise ValueError(f"configured monitor speed is not {SERIAL_SPEED}")


def platformio_ports(project: Path) -> list[str]:
    command = [str(project / "scripts" / "pio.sh"), "device", "list", "--json-output"]
    completed = subprocess.run(command, cwd=project, text=True, capture_output=True, check=False)
    if completed.returncode != 0:
        message = completed.stderr.strip() or completed.stdout.strip() or "unknown PlatformIO error"
        raise ValueError(f"PlatformIO could not list serial devices: {message}")
    try:
        devices = json.loads(completed.stdout)
    except json.JSONDecodeError as exc:
        raise ValueError("PlatformIO returned an invalid device list") from exc
    return [item["port"] for item in devices if isinstance(item, dict) and isinstance(item.get("port"), str)]


def select_port(requested_port: str | None, available_ports: list[str]) -> str:
    if requested_port:
        if requested_port not in available_ports:
            listed = ", ".join(available_ports) if available_ports else "none"
            raise ValueError(f"requested port {requested_port!r} is not reported by PlatformIO (reported: {listed})")
        return requested_port
    if len(available_ports) != 1:
        listed = ", ".join(available_ports) if available_ports else "none"
        raise ValueError(f"select --port explicitly; PlatformIO reported {len(available_ports)} devices ({listed})")
    return available_ports[0]


def redact_command_line(arguments: argparse.Namespace) -> str:
    parts = ["monitor.py", "--session-type", arguments.session_type]
    if arguments.port:
        parts.extend(["--port", arguments.port])
    parts.extend(["--duration", str(arguments.duration), "--require-button", arguments.require_button])
    return " ".join(parts)


def write_summary(
    path: Path,
    arguments: argparse.Namespace,
    project: Path,
    port: str | None,
    evidence: Evidence,
    result: Classification,
    started_at: datetime,
    ended_at: datetime,
    raw_path: Path,
    records_path: Path,
    validation_error: str | None,
) -> None:
    dependency = next((record for record in evidence.records if record["event"] == "dependency_identity"), {})
    connected = next((record for record in evidence.records if record["event"] == "controller_connected"), {})
    events = evidence.events()
    outcome = result.name
    if result.name == "successful_input" and arguments.session_type != "unknown":
        outcome = f"{arguments.session_type.replace('-', '_')}_success"

    lines = [
        "# ESP32 controller diagnostic summary",
        "",
        "## Reproducibility metadata",
        "",
        f"- Started (UTC): `{started_at.isoformat()}`",
        f"- Ended (UTC): `{ended_at.isoformat()}`",
        f"- Invocation: `{redact_command_line(arguments)}`",
        f"- Project: `{project}`",
        f"- PlatformIO environment / board: `{EXPECTED_ENVIRONMENT}` / `{EXPECTED_BOARD}`",
        "- Framework: `ESP-IDF with Arduino as a component`",
        f"- Serial port / baud: `{port or 'unavailable'}` / `{SERIAL_SPEED}`",
        "- Target controller / mode: `Mystiluck/Senze SZ-4015B` / `PS4-compatible`",
        f"- Observed controller model: `{connected.get('model_name', 'not observed')}`",
        f"- Bluepad32 / BTstack: `{dependency.get('bluepad32', 'not observed')}` / `{dependency.get('btstack', 'not observed')}`",
        f"- Raw transcript: `{raw_path.name}`",
        f"- Extracted records: `{records_path.name}`",
        "",
        "## Result",
        "",
        f"- Classification: `{result.name}`",
        f"- Session outcome: `{outcome}`",
        f"- Furthest evidenced stage: {result.furthest_stage}",
        f"- Interpretation: {result.explanation}",
    ]
    if validation_error:
        lines.append(f"- Validation/open error: `{validation_error}`")
    lines.extend(
        [
            "",
            "## Automated serial evidence",
            "",
            f"- Ordered events: `{', '.join(events) if events else 'none'}`",
            f"- Valid BTDIAG records: {len(evidence.records)}",
            f"- Malformed BTDIAG lines ignored: {evidence.malformed_btdiag_lines}",
            f"- Unsupported protocol records ignored: {evidence.unsupported_protocol_lines}",
            f"- Other-firmware boot records ignored: {evidence.firmware_mismatch_lines}",
            "",
            "## Automated build/upload evidence supplied to this run",
            "",
        ]
    )
    lines.extend(f"- {item}" for item in arguments.build_evidence)
    if not arguments.build_evidence:
        lines.append("- None. The default listener did not build or upload firmware.")
    lines.extend(["", "## User-confirmed physical observations", ""])
    lines.extend(f"- {item}" for item in arguments.user_observation)
    if not arguments.user_observation:
        lines.append("- None supplied. Physical button presses and indicator lights were not inferred from serial output.")
    lines.extend(
        [
            "",
            "## Verification boundary",
            "",
            "The classification is based only on valid structured serial records. Build/upload statements and user-reported physical actions are listed separately and do not replace connection-plus-input evidence.",
            "",
        ]
    )
    path.write_text("\n".join(lines), encoding="utf-8")


def output_paths(session_type: str) -> tuple[Path, Path, Path]:
    DIAGNOSTICS_DIR.mkdir(parents=True, exist_ok=True)
    stamp = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    stem = DIAGNOSTICS_DIR / f"{stamp}-{session_type}"
    suffix = 1
    while stem.with_suffix(".log").exists():
        stem = DIAGNOSTICS_DIR / f"{stamp}-{session_type}-{suffix}"
        suffix += 1
    return stem.with_suffix(".log"), stem.with_suffix(".jsonl"), stem.with_suffix(".md")


def monitor_serial(
    port: str,
    duration: float,
    required_button: str,
    evidence: Evidence,
    raw_file: TextIO,
    records_file: TextIO,
) -> Classification:
    if serial is None:
        raise SerialException("pyserial is not installed; install it for Python 3 or use the experiment's PlatformIO environment")

    interrupted = False

    def request_stop(_signum: int, _frame: Any) -> None:
        nonlocal interrupted
        interrupted = True

    old_sigint = signal.signal(signal.SIGINT, request_stop)
    old_sigterm = signal.signal(signal.SIGTERM, request_stop)
    try:
        with serial.Serial(port=port, baudrate=SERIAL_SPEED, timeout=0.2, exclusive=True) as connection:
            print(f"DIAG monitor_open port={port} baud={SERIAL_SPEED}; press EN/RST now", flush=True)
            deadline = time.monotonic() + duration
            pending = bytearray()
            while time.monotonic() < deadline and not interrupted:
                chunk = connection.read(max(connection.in_waiting, 1))
                if not chunk:
                    continue
                pending.extend(chunk)
                while b"\n" in pending:
                    raw_line, _, remainder = pending.partition(b"\n")
                    pending = bytearray(remainder)
                    line = raw_line.decode("utf-8", errors="replace").rstrip("\r")
                    raw_file.write(line + "\n")
                    raw_file.flush()
                    print(line, flush=True)
                    record = parse_btdiag_line(line, evidence)
                    if record is not None:
                        records_file.write(json.dumps(record, separators=(",", ":")) + "\n")
                        records_file.flush()
                        print(f"DIAG observed event={record['event']}", flush=True)
                        if has_requested_input(evidence.records, required_button):
                            return classify(evidence, True, required_button)
                    if record and record["event"] == "connection_timeout":
                        return classify(evidence, True, required_button)
            if pending:
                line = pending.decode("utf-8", errors="replace").rstrip("\r")
                raw_file.write(line + "\n")
                print(line, flush=True)
                record = parse_btdiag_line(line, evidence)
                if record is not None:
                    records_file.write(json.dumps(record, separators=(",", ":")) + "\n")
            return classify(evidence, True, required_button)
    finally:
        signal.signal(signal.SIGINT, old_sigint)
        signal.signal(signal.SIGTERM, old_sigterm)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--project", type=Path, default=EXPECTED_PROJECT, help=argparse.SUPPRESS)
    parser.add_argument("--environment", default=EXPECTED_ENVIRONMENT, help=argparse.SUPPRESS)
    parser.add_argument("--port", help="serial device reported by PlatformIO; auto-select only when unique")
    parser.add_argument("--duration", type=float, default=90.0, help="bounded listen duration in seconds (default: 90)")
    parser.add_argument(
        "--session-type", choices=("first-pair", "reconnection", "unknown"), default="unknown", help="physical test being performed"
    )
    parser.add_argument("--require-button", default="cross", help="button required after connection (default: cross)")
    parser.add_argument("--build-evidence", action="append", default=[], help="agent-observed build/upload fact; repeatable")
    parser.add_argument("--user-observation", action="append", default=[], help="user-confirmed physical observation; repeatable")
    return parser


def main() -> int:
    arguments = build_parser().parse_args()
    if arguments.duration <= 0:
        print("error: --duration must be greater than zero", file=sys.stderr)
        return 2

    started_at = datetime.now(timezone.utc)
    raw_path, records_path, summary_path = output_paths(arguments.session_type)
    evidence = Evidence()
    port: str | None = None
    validation_error: str | None = None
    result = classify(evidence, False, arguments.require_button)

    with raw_path.open("w", encoding="utf-8") as raw_file, records_path.open("w", encoding="utf-8") as records_file:
        try:
            project = arguments.project.resolve()
            validate_project(project, arguments.environment)
            port = select_port(arguments.port, platformio_ports(project))
            result = monitor_serial(port, arguments.duration, arguments.require_button, evidence, raw_file, records_file)
        except (OSError, SerialException, ValueError) as exc:
            validation_error = str(exc)
            result = classify(evidence, False, arguments.require_button)
            print(f"DIAG serial_unavailable: {validation_error}", file=sys.stderr, flush=True)

    ended_at = datetime.now(timezone.utc)
    write_summary(
        summary_path,
        arguments,
        arguments.project.resolve(),
        port,
        evidence,
        result,
        started_at,
        ended_at,
        raw_path,
        records_path,
        validation_error,
    )
    print(f"DIAG classification={result.name} furthest_stage={result.furthest_stage}", flush=True)
    print(f"DIAG transcript={raw_path}", flush=True)
    print(f"DIAG records={records_path}", flush=True)
    print(f"DIAG summary={summary_path}", flush=True)
    return 2 if result.name == "serial_unavailable" else 0


if __name__ == "__main__":
    raise SystemExit(main())
