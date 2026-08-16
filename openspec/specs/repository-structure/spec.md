# Repository Structure

## ADDED Requirements

### Requirement: Curriculum repository layout
The repository SHALL provide top-level directories for electronics reference materials, standalone experiments, syllabus and teacher materials, a PlatformIO Arduino Uno template, and PlatformIO lesson projects.

#### Scenario: Browse the repository root
- **WHEN** a contributor lists the repository root
- **THEN** they find `electronics-reference/`, `experiments/`, `syllabus-and-teacher-materials/`, `platformio-uno-template/`, and `platformio-lessons/`

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
