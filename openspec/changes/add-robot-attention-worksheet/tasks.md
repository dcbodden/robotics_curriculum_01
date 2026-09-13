## 1. Student worksheet

- [x] 1.1 Revise `robot-attention-worksheet.html` in lesson 05 to three spacious US Letter pages: background/explanations, hypothesis/procedure, and observations/conclusion, preserving monochrome print styling, name/date fields, and usable writing space.
- [x] 1.2 Add fifth-grade robot-attention text, faithful excerpts explaining lesson 04's read/map/apply/report/delay sequence, and the near-one-second possible command lag.
- [x] 1.3 Add the contrasting ISR excerpt and timer-reminder explanation, a correctly labeled timing diagram, and the distinction between 100 ms command checks and one-second reporting while delay() remains in lesson 05.
- [x] 1.4 Replace the ambiguous “My prediction” area with the 1000 ms versus 100 ms investigation question, a fifth-grade hypothesis definition, and at least four lines for a student-written hypothesis and reason before testing. Add numbered lesson 04/05 experimental steps, at least three paired B1/RV low/high trial records, and a conclusion asking whether 100 ms responses were better and whether trial evidence supports the hypothesis; allow similar, mixed, or unclear results. Retain reflection on old commands, ten checks per second, and collision/damage/overheating information needs.

## 2. Teacher guidance and discovery

- [x] 2.1 Create `robot-attention-teacher-guide.md` with preparation, a short lesson sequence, expected answers, the teacher example hypothesis about more noticeable lag with 1000 ms checks, guidance to accept student-written hypotheses and evidence-based conclusions, comparable numbered trial instructions, and links to existing B1/RV wiring, build/upload, and safety guidance.
- [x] 2.2 Explain motor coasting and starting thresholds, command updates versus PWM pulses and serial messages, the need for additional sensors in transfer examples, and the limits of the demonstration's checking rate.
- [x] 2.3 Link worksheet source, PDF, and teacher guide from lessons 04 and 05 and identify the comparison in `platformio-uno-lessons/README.md`.

## 3. Printable output and verification

- [x] 3.1 Generate the matching `robot-attention-worksheet.pdf` from the HTML and document the reproducible generation command in the teacher guide.
- [x] 3.2 Inspect all three rendered PDF pages for US Letter sizing, exactly three pages, readable black-and-white text/code/diagram, unclipped content, intact rows, at least four hypothesis/reason lines, and sufficient observation/conclusion handwriting space; revise and regenerate if needed.
- [x] 3.3 Review explanations and teacher answers against both existing programs, verify B1/RV terminology and local links, and confirm firmware and wiring requirements remain unchanged.
- [x] 3.4 Validate the OpenSpec change and summarize completed artifact checks; report any unperformed physical demonstration separately from document verification.

## Verification record

- Regenerated the PDF from the HTML using the command documented in the teacher guide.
- Confirmed exactly three US Letter pages (612 × 792 points) and visually inspected every rendered page: readable monochrome text/code/timing diagram, no clipping or split trial rows, name/date fields, four hypothesis lines, and usable observation/conclusion writing space.
- Reviewed student explanations and teacher answers against both programs: lesson 04's 1000 ms control delay, lesson 05's 100 ms ISR, A0/D3 mapping, continuing hardware PWM, and roughly one-second reporting in both.
- Verified all 23 local links in the teacher guide, both lesson READMEs, and lesson index, including heading anchors. Confirmed the existing B1 two-AA motor supply and Arduino-powered RV voltage divider terminology; firmware, configuration, and wiring requirements are unchanged.
- `openspec validate add-robot-attention-worksheet --strict` and `git diff --check` passed.
- Physical demonstration: not performed. Motor-response expectations are teacher guidance, not measured results; the student will record actual observations during the B1/RV trials.
