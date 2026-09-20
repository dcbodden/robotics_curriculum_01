#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
project_dir="$(cd "${script_dir}/.." && pwd)"
venv_dir="${project_dir}/.tooling/platformio-core-6.1.19"
core_dir="${project_dir}/.tooling/platformio-home-6.1.19"

if [[ ! -x "${venv_dir}/bin/pio" ]] || \
    ! "${venv_dir}/bin/python" -c \
        'from importlib.metadata import version; assert version("platformio") == "6.1.19"; assert version("click") == "8.1.7"' \
        >/dev/null 2>&1; then
    python3 -m venv "${venv_dir}"
    "${venv_dir}/bin/python" -m pip install \
        --requirement "${project_dir}/requirements-tooling.txt"
fi

cd "${project_dir}"
exec env PLATFORMIO_CORE_DIR="${core_dir}" "${venv_dir}/bin/pio" "$@"
