# beginner-curriculum-progression Specification

## Purpose

Defines a consistent, concrete-first route through the beginner curriculum so each new programming or electronics idea leads quickly to an observable result without hiding required safety guidance.

## Requirements

### Requirement: Canonical beginner learning route
The curriculum SHALL present one primary beginner route in the repository guide and relevant collection indexes: multimeter foundations, coding fundamentals, the first Arduino digital-output circuit, Arduino ADC variable delay, manual voltage-mapping reinforcement, PWM motor control, responsive interrupt-driven control, and coordinated joystick motor/servo control. Optional activities SHALL be identified without contradicting prerequisites on the primary route.

#### Scenario: Plan the beginner sequence
- **WHEN** a teacher follows the primary route from any linked curriculum index
- **THEN** each next activity satisfies the prerequisites stated by that activity and matches the order shown by the other indexes

### Requirement: Layered beginner lesson presentation
Each beginner lesson SHALL place a short Core Mission before optional extensions and deeper reference material. The Core Mission SHALL identify its immediate goal, required preparation and safety checkpoints, the minimum steps to produce one observable result, and a short observation prompt. Completion of the Core Mission SHALL NOT depend on reading optional implementation details, while essential safety instructions SHALL remain in the required path.

#### Scenario: Complete a core mission
- **WHEN** a student opens a lesson for the first time
- **THEN** the student can locate and complete the required path to one observable result without first reading optional technical explanations or exhaustive troubleshooting

#### Scenario: Preserve required safety guidance
- **WHEN** a lesson involves powered wiring, measurement, or moving hardware
- **THEN** its Core Mission retains power-removal instructions, teacher checkpoints, stop conditions, and other safety steps required before observation

### Requirement: Concrete-first learning cycle
Beginner lessons SHALL guide the student through an initial working observation before asking for detailed code explanation, then identify one main concept and offer a bounded change, comparison, or second trial that supports prediction and explanation from evidence. The bounded activity SHALL avoid source modification when changing hardware or firmware would add unnecessary risk.

#### Scenario: Learn from a visible result
- **WHEN** a student encounters a new programming or electronics concept
- **THEN** the lesson first provides a guided observable example and then asks the student to connect one named concept to a prediction, safe change, comparison, or explanation

### Requirement: Student-focus and reference distinction
Lessons containing incidental implementation complexity SHALL clearly identify the small code or circuit portion the student is expected to understand and place helper mechanics, tool internals, component selection details, and advanced implementation explanations in teacher or deeper-reference material.

#### Scenario: Read a lesson with helper code
- **WHEN** safe or correct operation requires code beyond the current learning goal
- **THEN** the student can distinguish the focus steps from helper code and is not assessed on the helper's internal mechanics
