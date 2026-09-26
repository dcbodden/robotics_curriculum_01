#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
project_dir="$(cd -- "${script_dir}/.." && pwd)"
check_dir="$(mktemp -d)"
trap 'rm -rf -- "${check_dir:?}"' EXIT

compile_flags=(
    -std=c++11
    -Wall
    -Wextra
    -Werror
    -I"${project_dir}/main"
)

g++ "${compile_flags[@]}" "${script_dir}/test_control_mapping.cpp" -o "${check_dir}/mapping-check"
g++ "${compile_flags[@]}" "${script_dir}/test_control_policy.cpp" -o "${check_dir}/policy-check"
g++ "${compile_flags[@]}" "${script_dir}/test_actuator_commands.cpp" -o "${check_dir}/actuator-check"

"${check_dir}/mapping-check"
"${check_dir}/policy-check"
"${check_dir}/actuator-check"
