#!/usr/bin/env bash
set -euo pipefail

readonly template_commit="d07a9385f46f7215f51fc3eb5e40c5a484cfe102"
readonly template_sha256="6ced5c5b4c6af92f4412bff4d7bd8149f5073010d375f95b54482db1c406364a"
readonly arduino_commit="ac961f671abd5ae1da0a15fd4bee71ed807c2cf3"
readonly arduino_sha256="8d0156e0aa38976e2f5ed7ad31a9c7787a6465f09844834901bdff19af5fe23d"

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
project_dir="$(cd -- "${script_dir}/.." && pwd)"
component_dir="${project_dir}/components"
stamp_path="${component_dir}/.dependency-stamp"

required_components=(
    arduino
    bluepad32
    bluepad32_arduino
    btstack
    cmd_nvs
    cmd_nvs_4.4
    cmd_system
    cmd_system_4.4
)

dependencies_are_current=true
for component in "${required_components[@]}"; do
    if [[ ! -d "${component_dir}/${component}" ]]; then
        dependencies_are_current=false
        break
    fi
done

if [[ "${dependencies_are_current}" == true ]] &&
   grep -Fqx "template=${template_commit}" "${stamp_path}" 2>/dev/null &&
   grep -Fqx "arduino=${arduino_commit}" "${stamp_path}" 2>/dev/null; then
    echo "Pinned ESP-IDF components are already present."
    exit 0
fi

for component in "${required_components[@]}"; do
    if [[ -e "${component_dir}/${component}" ]]; then
        echo "Refusing to replace partial or unverified component: ${component_dir}/${component}" >&2
        echo "Move that generated directory aside, then run this script again." >&2
        exit 1
    fi
done

temp_dir="$(mktemp -d)"
trap 'rm -rf -- "${temp_dir:?}"' EXIT

template_archive="${temp_dir}/bluepad32-template.tar.gz"
arduino_archive="${temp_dir}/arduino-esp32.tar.gz"

curl -L --fail --retry 3 \
    -o "${template_archive}" \
    "https://github.com/ricardoquesada/esp-idf-arduino-bluepad32-template/archive/${template_commit}.tar.gz"
curl -L --fail --retry 3 \
    -o "${arduino_archive}" \
    "https://github.com/espressif/arduino-esp32/archive/${arduino_commit}.tar.gz"

printf '%s  %s\n' "${template_sha256}" "${template_archive}" | sha256sum --check --strict
printf '%s  %s\n' "${arduino_sha256}" "${arduino_archive}" | sha256sum --check --strict

mkdir -p "${temp_dir}/template" "${temp_dir}/arduino" "${component_dir}"
tar -xzf "${template_archive}" --strip-components=1 -C "${temp_dir}/template"
tar -xzf "${arduino_archive}" --strip-components=1 -C "${temp_dir}/arduino"

for component in bluepad32 bluepad32_arduino btstack cmd_nvs cmd_nvs_4.4 cmd_system cmd_system_4.4; do
    cp -a "${temp_dir}/template/components/${component}" "${component_dir}/${component}"
done
cp -a "${temp_dir}/arduino" "${component_dir}/arduino"

printf 'template=%s\narduino=%s\n' "${template_commit}" "${arduino_commit}" > "${stamp_path}"
echo "Installed checksum-verified ESP-IDF components in ${component_dir}."
