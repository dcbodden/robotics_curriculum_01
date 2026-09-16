# Verification — beginner learning scaffolding

Performed on the workstation:

- Built all four documented C sources with `gcc -std=c11 -Wall -Wextra -Werror`. The original Hello, World output, multiword name greeting, empty-name and end-of-input messages matched the source and guides.
- Ran the finite counter and observed `1, 2, 3, 4, 5` followed by program exit after about five seconds. Ran the repeating counter and observed `1, 2, 3, 4, 5, 1` while it continued; then stopped it.
- Built the coding lesson 04 PlatformIO `uno` project and Uno lessons 01–06 successfully. Their documented `uno` environment and 9,600-baud Monitor setting match each `platformio.ini`. No final firmware source was changed for this verification.
- Reviewed the twelve revised student guides, including the four gated missions within Uno lesson 06. Each required path names an observable result before detailed explanation, states a safe stopping point, and separates the focus concept from helper or teacher reference. The hardware paths retain power-removal steps, teacher checkpoints, approved circuits or supplies, and motion or meter safeguards. The root and three collection indexes follow the same prerequisite route.
- Checked 148 local Markdown links and anchors across the root guide and revised collections; all targets exist. Strict OpenSpec validation and `git diff --check` passed.

Not performed in this session: Arduino uploads; physical LED, meter, RV, motor, or servo observations; circuit wiring or power-supply measurements. Builds and document review do not establish physical hardware behavior. A teacher must follow each lesson's checkpoints before a student runs the hardware activities.
