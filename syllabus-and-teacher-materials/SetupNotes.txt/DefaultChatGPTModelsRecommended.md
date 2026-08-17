For Arduino + PlatformIO in VS Code, I recommend **GPT‑5.6 Terra with medium reasoning** as your everyday model.

* **Terra + Medium:** writing drivers, refactoring C/C++, configuring `platformio.ini`, interpreting compiler output, and working with Arduino libraries.
* **Sol + High:** difficult bugs involving timing, interrupts, concurrency, memory corruption, protocols, linker errors, or unfamiliar datasheets.
* **Luna + Low/Medium:** quick boilerplate, comments, repetitive pin mappings, and straightforward edits.

Embedded development often requires more judgment than ordinary application code because the model must reason about limited RAM/flash, register behavior, timing, electrical assumptions, and board-specific differences. Therefore, I’d favor **Sol** when correctness matters more than speed; otherwise, **Terra** is the best balance.

In the Codex composer, select:

> **5.6 Terra → Medium**

OpenAI describes Terra as the everyday all-rounder and Sol as the strongest option for complex coding. Medium reasoning is the documented balance between speed and depth. [Official OpenAI model guide](https://developers.openai.com/codex/models)

A useful first prompt would be:

> Inspect this PlatformIO project, including `platformio.ini`, the selected board/framework, library dependencies, and source files. Before changing code, identify RAM, flash, timing, voltage, pin, interrupt, and board-compatibility constraints. Use PlatformIO build output to verify all changes. Do not assume hardware behavior that is not established by the board or component documentation.

One important limitation: Codex can run `pio run` and analyze serial logs, but it cannot infer the physical wiring reliably unless you provide the schematic, pin mapping, board model, and attached-component details.
