#!/usr/bin/env python3
"""Deterministic checks for BTDIAG parsing and session classification."""

from __future__ import annotations

import importlib.util
import json
import sys
import unittest
from pathlib import Path
from typing import Any


MONITOR_PATH = Path(__file__).resolve().parents[1] / "scripts" / "monitor.py"
SPEC = importlib.util.spec_from_file_location("esp32_controller_monitor", MONITOR_PATH)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError(f"cannot load monitor helper from {MONITOR_PATH}")
monitor = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = monitor
SPEC.loader.exec_module(monitor)


def record(event: str, elapsed_ms: int, **fields: Any) -> dict[str, Any]:
    value: dict[str, Any] = {
        "protocol_version": monitor.EXPECTED_PROTOCOL_VERSION,
        "elapsed_ms": elapsed_ms,
        "event": event,
    }
    if event == "firmware_started":
        value["firmware"] = monitor.EXPECTED_FIRMWARE
    value.update(fields)
    return value


def diagnostic_line(event: str, elapsed_ms: int, **fields: Any) -> str:
    return monitor.BTDIAG_PREFIX + json.dumps(record(event, elapsed_ms, **fields))


def parsed_evidence(*lines: str) -> monitor.Evidence:
    evidence = monitor.Evidence()
    for line in lines:
        monitor.parse_btdiag_line(line, evidence)
    return evidence


BOOT = diagnostic_line("firmware_started", 0)
READY = diagnostic_line("bluetooth_ready", 10)
ACCEPTING = diagnostic_line("accepting_connections", 20)
CONNECTED = diagnostic_line("controller_connected", 30, model_name="Wireless Controller")
DISCONNECTED = diagnostic_line("controller_disconnected", 40, model_name="Wireless Controller")
TIMEOUT = diagnostic_line("connection_timeout", 75020)
NEUTRAL_INPUT = diagnostic_line(
    "controller_first_input", 40, valid_report=True, pressed_buttons="none"
)
CROSS_INPUT = diagnostic_line(
    "controller_input", 50, valid_report=True, pressed_buttons="l1,cross"
)


class ClassificationTests(unittest.TestCase):
    def assert_classification(
        self,
        expected: str,
        lines: tuple[str, ...] = (),
        *,
        serial_opened: bool = True,
    ) -> None:
        result = monitor.classify(parsed_evidence(*lines), serial_opened, "cross")
        self.assertEqual(expected, result.name)

    def test_every_diagnostic_classification(self) -> None:
        cases = {
            "serial_unavailable": (False, ()),
            "missing_experiment_boot": (True, ("I (12) BTstack: raw stack log",)),
            "bluetooth_initialization_incomplete": (True, (BOOT,)),
            "waiting_for_controller": (True, (BOOT, READY, ACCEPTING)),
            "pairing_timeout": (True, (BOOT, READY, ACCEPTING, TIMEOUT)),
            "connected_without_input": (
                True,
                (BOOT, READY, ACCEPTING, CONNECTED, NEUTRAL_INPUT),
            ),
            "premature_disconnection": (
                True,
                (BOOT, READY, ACCEPTING, CONNECTED, DISCONNECTED),
            ),
            "successful_input": (
                True,
                (BOOT, READY, ACCEPTING, CONNECTED, NEUTRAL_INPUT, CROSS_INPUT),
            ),
        }

        for expected, (serial_opened, lines) in cases.items():
            with self.subTest(expected=expected):
                self.assert_classification(expected, lines, serial_opened=serial_opened)

    def test_input_must_be_valid_requested_and_after_connection(self) -> None:
        invalid_input = diagnostic_line(
            "controller_input", 50, valid_report=False, pressed_buttons="cross"
        )
        before_connection = parsed_evidence(BOOT, READY, ACCEPTING, CROSS_INPUT, CONNECTED)
        invalid_after_connection = parsed_evidence(
            BOOT, READY, ACCEPTING, CONNECTED, invalid_input
        )

        self.assertEqual(
            "connected_without_input",
            monitor.classify(before_connection, True, "cross").name,
        )
        self.assertEqual(
            "connected_without_input",
            monitor.classify(invalid_after_connection, True, "cross").name,
        )

    def test_bluetooth_ready_without_accepting_is_initialization_incomplete(self) -> None:
        self.assert_classification(
            "bluetooth_initialization_incomplete", (BOOT, READY)
        )


class ParserTests(unittest.TestCase):
    def test_malformed_and_interleaved_lines_are_ignored(self) -> None:
        evidence = monitor.Evidence()
        wrong_firmware = record("firmware_started", 3, firmware="other-project")
        lines = [
            "I (12) BTstack: controller inquiry started",
            BOOT,
            "W (18) gap: lower-level warning",
            monitor.BTDIAG_PREFIX + "{not-json}",
            monitor.BTDIAG_PREFIX + json.dumps(["not", "an", "object"]),
            monitor.BTDIAG_PREFIX + json.dumps({"protocol_version": 1, "elapsed_ms": 2}),
            monitor.BTDIAG_PREFIX
            + json.dumps({"protocol_version": 1, "event": "bluetooth_ready"}),
            monitor.BTDIAG_PREFIX
            + json.dumps(record("bluetooth_ready", 4) | {"protocol_version": 99}),
            monitor.BTDIAG_PREFIX + json.dumps(wrong_firmware),
            "BTstack says the text BTDIAG but supplies no record",
            READY,
        ]

        parsed = [
            result
            for line in lines
            if (result := monitor.parse_btdiag_line(line, evidence)) is not None
        ]

        self.assertEqual(["firmware_started", "bluetooth_ready"], evidence.events())
        self.assertEqual(2, len(parsed))
        self.assertEqual(5, evidence.malformed_btdiag_lines)
        self.assertEqual(1, evidence.unsupported_protocol_lines)
        self.assertEqual(1, evidence.firmware_mismatch_lines)


if __name__ == "__main__":
    unittest.main()
