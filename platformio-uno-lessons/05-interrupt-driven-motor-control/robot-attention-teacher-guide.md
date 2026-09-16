# Robot Attention: Teacher Guide

Use the [three-page student worksheet](robot-attention-worksheet.html) to investigate whether a motor responds more promptly when its RV control is checked every 100 ms rather than about once per second. The learning goal is: **follow the plan and keep checking what has changed**. Plan approximately 20–30 minutes after the circuit is ready.

## Prepare the demonstration

Use the Arduino Uno, Snap Circuits B1 two-AA battery holder, three-terminal RV control, and protected motor circuit already documented in lessons 04 and 05. Keep the same circuit for both programs.

- Follow lesson 04's [motor and gate connections](../04-adc-pwm-motor-control/README.md#connect-the-low-side-motor-and-gate-paths), [RV-to-A0 connections](../04-adc-pwm-motor-control/README.md#connect-the-rv-voltage-divider-to-a0), and [flywheel diode instructions](../04-adc-pwm-motor-control/README.md#add-the-1n5817-flywheel-diode).
- Complete the [safety check before powering the circuit](../04-adc-pwm-motor-control/README.md#complete-the-safety-check-before-powering-the-circuit). Remove both USB and B1 power before wiring changes. Verify MOSFET terminals, gate resistors, shared ground, separate positive supplies, RV wiring, and diode polarity.
- Secure the bare motor with no wheel, propeller, gear, or other shaft attachment. Keep hands, hair, clothing, and objects clear of the shaft. The teacher approves the circuit before power is applied and keeps B1 off during every upload.
- Review lesson 04's [Core Mission build, upload, and monitor instructions](../04-adc-pwm-motor-control/README.md#core-mission-turn-the-rv-watch-the-motor-command) and lesson 05's [paired program comparison](README.md#core-mission-can-the-motor-notice-an-rv-change-sooner). Both monitors use 9600 baud. For register and shared-data details, see the [timer teacher reference](timer-teacher-reference.md).
- Have all three worksheet pages available. With B1 off and USB connected after wiring approval, choose recognizable low and high RV positions. Use a low position near a zero PWM command and a high position that gives clear motor motion in the approved setup. Return to low before enabling B1.

If the motor fails to turn when commanded to run, parts become hot, unusual smells, smoke, or sparks appear, or wiring moves unexpectedly, switch B1 off and disconnect USB immediately. Follow the existing safety instructions before considering another trial.

## Lesson sequence

1. **Connect to everyday attention (3 minutes).** Discuss walking across a room: a destination does not remove the need to notice people or chairs. Read page 1 together. Identify the read, map, apply, report, and wait steps in lesson 04, then the timer's checking routine in lesson 05. Keep the discussion focused on how often new information is used.
2. **Write a hypothesis (4 minutes).** Read page 2's investigation question. Explain that a hypothesis is a prediction we can test. Ask the student to write what they expect and why before either motor test. Accept either predicted outcome when it answers the question and has a reason; help clarify vague wording without supplying the answer.
3. **Run the comparison (10–15 minutes).** Follow the numbered procedure below and record observations on page 3 as each trial finishes. Encourage concrete observations of sound or motion; no stopwatch measurements are required.
4. **Draw a conclusion (5 minutes).** Ask the student to compare the two columns, choose a response, cite trial evidence, and revisit their hypothesis. Complete the checking questions and one robot-problem reflection.

### Teacher example hypothesis

> A 1000 ms delay between checks of the RV position will cause a more noticeable lag in motor-speed response than checking every 100 ms.

Use this as preparation guidance. Give the student the question and opportunity to formulate their own hypothesis. A useful prediction might say which program will react sooner and connect that expectation to the time between checks.

## Numbered trial procedure

1. With B1 off and wiring approved, build and upload lesson 04, then open its 9600-baud monitor. Return the RV to the chosen low position and confirm the reported PWM command is near zero. Recheck that the motor is secured and its shaft is clear before enabling B1.
2. Just after a new serial message appears, move the RV quickly **low to high**. Hold the new position until the response is clear, and record lesson 04's trial 1 observation.
3. Just after a later message, move **high to low**, hold, and record trial 2. Repeat **low to high** for trial 3. Use the same low and high positions each time.
4. Switch B1 off, close Monitor, and unplug USB. Leave every wire unchanged and confirm the same circuit while both sources are off. Build lesson 05, reconnect USB with B1 still off, upload, and reopen Monitor at 9600 baud. Return to low and confirm the near-zero command before enabling B1 after teacher approval.
5. Repeat the same three movements, each just after a message: **low to high**, **high to low**, **low to high**. Record each observation in lesson 05's column. Keep the circuit, RV positions, movements, and observation method comparable.
6. Switch B1 off, close the monitor, and disconnect USB. Compare the recorded responses and complete the conclusion.

These three paired trials match the worksheet. The existing lesson 05 README offers a longer comparison if additional repetitions are useful; record those separately rather than overwrite the original observations.

## Expected answers and evidence

| Worksheet item | Teacher guidance |
| --- | --- |
| Investigation and hypothesis | A testable prediction compares the noticeable lag with 1000 ms and 100 ms checks. The student's wording and predicted outcome can differ from the teacher example. |
| Trial observations | Expect a more noticeable wait with lesson 04 and a quicker response with lesson 05, particularly after moving just after a report. These are expectations, not results to fill in for the student. Accept descriptions such as “the sound changed quickly” or “I noticed a pause.” |
| Did lesson 05 respond sooner? | The expected answer is yes, but accept about the same, slower, mixed, or hard to tell when consistent with the student's records. Ask which trials led to the answer. |
| Evidence and hypothesis support | A useful answer refers to recorded observations, for example “I wrote ‘noticeable wait’ for 04 and ‘quick change’ for 05 in trials 1 and 3.” Require the student to use their actual evidence. Similar or unclear observations can leave the hypothesis unsupported or inconclusive. |
| Power setting during lesson 04's wait | The previous PWM power setting. Moving the RV does not update the command until the next reading. |
| Checks per second | 1000 ÷ 100 = **10**. |
| Robot-problem reflection | Accept an appropriate information source and reason for frequent checks: obstacle distance to respond to a blocked path, a control reading to notice changed instructions, or temperature to notice a hot motor. The RV activity tests control changes; the other examples are discussion scenarios. |

Judge the reasoning by whether the prediction is testable, the observations are recorded honestly, and the conclusion follows the evidence. A hypothesis need not be supported for the experiment to be useful. If the result is unclear, discuss what another matching trial could help establish instead of replacing the recorded answer with the expected one.

## Interpret the timing and motor behavior

### Three different rhythms

| What happens | Lesson 04 | Lesson 05 |
| --- | --- | --- |
| Read the RV and apply its PWM power command | About once per second; the loop waits 1000 ms between passes. | Every 100 ms in the ISR: ten checks and command updates per second. |
| Generate PWM on/off pulses on D3 | Hardware continues the selected power setting during the wait. | Hardware continues the selected power setting between ISR updates. Ten updates per second does not mean ten PWM pulses per second. |
| Print numbers in the serial monitor | About once per second. | About once per second; reports show snapshots, so they do not display every ISR update. |

Lesson 05 still calls `delay(REPORT_DELAY_MS)` in `loop()`. Its timer interrupt briefly pauses regular work, reads and updates the control, and returns. The two routines do not run simultaneously. The useful observation is a motor response between computer messages; the message pace alone cannot demonstrate the difference in checking rates.

Lesson 04's lag depends on when the RV moves. Moving just after a reading can leave almost a full second before the next command update, while moving just before a reading leaves a short wait. Moving just after a serial report makes the longer wait easier to observe. Lesson 05 normally applies the new command at its next 100 ms check, but that is a command-update interval, not a promise that the motor reaches a new speed within 100 ms.

### Coasting and starting thresholds

A spinning motor can **coast** after its command drops to zero. Listen for when its sound begins to change rather than assume all continued motion means the program missed the RV change. A faster command update does not provide instant stopping or active braking in this circuit.

A motor can also have a **starting threshold**: very small nonzero PWM commands may not be enough to start it. Its observed speed may not change evenly with the command numbers. Use the same clearly separated low and high RV positions for both programs; tiny changes can make the comparison hard to judge. Never hold the shaft or deliberately stall the motor to create a response. Follow the preparation safety check if it fails to turn when commanded to run.

If these physical effects obscure the comparison, accept “hard to tell,” discuss the difference between the power command and actual motion, and consider additional matching trials only after checking the setup. Preserve the student's original records.

## Generate and print the worksheet

The [editable HTML](robot-attention-worksheet.html) is the source for the [printable PDF](robot-attention-worksheet.pdf). After changing the HTML, regenerate the PDF with Google Chrome. From this lesson's directory, run:

```bash
google-chrome --headless --no-sandbox --disable-gpu \
  --no-pdf-header-footer \
  --print-to-pdf="$PWD/robot-attention-worksheet.pdf" \
  "file://$PWD/robot-attention-worksheet.html"
```

The HTML sets US Letter paper and page breaks. Check that the regenerated PDF has exactly three pages and inspect every page for clipping and handwriting space. Print all three pages at actual size (100%), in black and white, on US Letter paper. Chrome is only needed to regenerate the PDF; students can use the printed copy without a browser or IDE.

## Transfer the idea to robot safety

The RV is a changing **control input**. Neither program measures obstacle distance or motor temperature. Discuss those examples without creating collisions or overheating the motor.

| Robot problem | Information it would need | Possible response after checking |
| --- | --- | --- |
| An obstacle appears in its path | An obstacle or distance sensor reading | Reduce movement, stop, or change direction. |
| A person changes the control instruction | A fresh control reading, like the RV reading here | Apply the new power command sooner. |
| A motor becomes too hot | A temperature sensor reading | Reduce or remove motor power and signal a problem. |

Checking helps only when the robot has the appropriate information and instructions for responding to it. **Ten checks per second is this demonstration's rate**, not a universally safe rate for robots. A suitable rate depends on how fast the robot and surroundings change, sensor behavior, processing time, and how long movement takes to slow or stop. Frequent checks can reduce the time spent acting on old information, but they do not guarantee collision avoidance or prevent every kind of damage.
