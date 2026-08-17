## 1. Lesson Project Structure

- [x] 1.1 Create `platformio-uno-lessons/` with a short README explaining the numbered, standalone lesson convention.
- [x] 1.2 Create `platformio-uno-lessons/01-gpio-transistor-switch/` as an independent Arduino Uno PlatformIO project using the existing `atmelavr`, `uno`, and `arduino` configuration.
- [x] 1.3 Add the PlatformIO IDE extension recommendation to the standalone lesson project.

## 2. GPIO Program

- [x] 2.1 Implement the lesson sketch with digital pin 8 configured as an output and alternating HIGH and LOW states.
- [ ] 2.2 Hold each output state for 2,000 milliseconds and use beginner-readable names and comments.

## 3. Student Instructions

- [ ] 3.1 Document the two connections to the existing Snap Circuits assembly: shared ground and Arduino D8 to the NPN base control point, including power-off and teacher-verification cautions.
- [ ] 3.2 Add a fifth-grade-friendly digital multimeter activity that measures D8 relative to shared ground during both two-second states and records the corresponding LED state.
- [ ] 3.3 Add numbered VS Code and PlatformIO instructions for connecting the Uno with a USB data cable, opening the lesson directory, building, uploading, and recognizing success.
- [ ] 3.4 Add concise troubleshooting for USB data cables, board detection, upload-port access, and Linux serial permissions.

## 4. Repository Documentation

- [ ] 4.1 Update the root repository documentation to include `platformio-uno-lessons/` and identify the first lesson.
- [ ] 4.2 Update the Uno template README to direct lesson authors and students to the separate lessons collection.

## 5. Verification

- [ ] 5.1 Build the standalone lesson with PlatformIO and confirm it targets an Arduino Uno without compiling another project.
- [ ] 5.2 Review the student procedure against the delta specs, including the two-second timing, Snap Circuits assumptions, multimeter comparison, and upload flow.
- [ ] 5.3 Run strict OpenSpec validation for `add-uno-gpio-transistor-lesson` and resolve any reported issues.
