## MODIFIED Requirements

### Requirement: Standalone electronics fundamentals collection
The curriculum SHALL provide `electronics-fundamentals-lessons/` as a collection of numbered, non-code lessons, beginning with one multimeter and Ohm's law lesson and one manual voltage-mapping lesson. Its index SHALL place both activities consistently within the repository's canonical beginner route.

#### Scenario: Follow the recommended lesson sequence
- **WHEN** a teacher plans the complete beginner curriculum
- **THEN** the multimeter and Ohm's law lesson comes before coding fundamentals and external Arduino circuits, the first Arduino digital-output lesson comes before the ADC variable-delay lesson, and manual voltage mapping follows the ADC lesson as reinforcement

#### Scenario: Complete a fundamentals lesson without firmware
- **WHEN** a student completes either fundamentals lesson
- **THEN** the student does not need to build or upload firmware for that activity
