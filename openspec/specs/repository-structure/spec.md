# Repository Structure

## Purpose

Provide an initial directory structure to support curriclum development.

## Requirements

### Requirement: Curriculum repository layout
The repository SHALL provide top-level directories for electronics reference materials, standalone experiments, syllabus and teacher materials, a PlatformIO Arduino Uno template, standalone PlatformIO Arduino Uno lessons, and an ESP32 robotics PlatformIO template.

#### Scenario: Browse the repository root
- **WHEN** a contributor lists the repository root
- **THEN** they find `electronics-reference/`, `experiments/`, `syllabus-and-teacher-materials/`, `platformio-uno-template/`, `platformio-uno-lessons/`, and `platformio-esp32-robotics-template/`

### Requirement: OpenSpec planning artifacts
The repository SHALL keep lightweight OpenSpec artifacts for durable requirements and structured changes.

#### Scenario: Review project planning assets
- **WHEN** a contributor opens `openspec/`
- **THEN** they find stable requirements under `openspec/specs/` and change records under `openspec/changes/`

### Requirement: Arduino Uno starter template
The repository SHALL provide a PlatformIO template configured for the Arduino Uno using the `atmelavr`, `uno`, and `arduino` platform/board/framework tuple.

#### Scenario: Inspect the template configuration
- **WHEN** a contributor opens `platformio-uno-template/platformio.ini`
- **THEN** the configuration targets PlatformIO `platform = atmelavr`, `board = uno`, and `framework = arduino`

### Requirement: ESP32 robotics starter template
The repository SHALL provide an ESP32-WROOM-32 PlatformIO template for advanced robotics work using the OV7670 camera and DRV8833 motor driver as core integration targets.

#### Scenario: Inspect the ESP32 robotics template
- **WHEN** a contributor opens `platformio-esp32-robotics-template/`
- **THEN** they find starter material for both `fpv-wifi-rc-car/` and `semi-autonomous-robotics/`
