# Lesson 01: Multimeter and Ohm's Law

In this standalone, non-code lesson, you will use a digital multimeter with a low-voltage circuit. You will observe what happens when a circuit path is open or closed, practice measuring electrical quantities, and compare measured current with a value calculated using Ohm's law.

## Learning Goals

By the end of the lesson, you will be able to:

- describe the difference between an open circuit and a closed circuit;
- recognize voltage, resistance, and current as different measurements;
- use voltage and resistance to calculate current; and
- compare a calculated result with a meter reading.

## Safety Rules — Low Voltage Only

> **Use only the B1 holder with its two teacher-approved AA cells and the B1/R1/S2 circuit in this lesson. Never place the probes in a household or wall outlet, power strip, appliance, or any other power source. Stop and tell the teacher if a part, lead, or meter is damaged, hot, wet, or smells unusual.**

- Remove both AA cells before building, changing a connection, or changing to or from current mode.
- Measure resistance only when B1 is empty and R1 is isolated.
- Measure voltage in parallel only while the red lead is in the voltage/resistance jack.
- Measure current only in the teacher-verified series path. Never place a current-mode meter directly across B1 or another component.
- Turn the meter off after each measurement. After current measurement, remove the cells before returning the red lead to the voltage/resistance jack.

## Teacher Preparation — Teacher Only

Use the [`sample_circuit/base_layout.jpg`](sample_circuit/base_layout.jpg) photo as the layout reference. The circuit uses only these three Snap Circuits parts:

| Part ID | Part | What to verify |
| --- | --- | --- |
| B1 | Battery holder for two AA cells | Record the installed cell type, nominal total voltage, and measured battery voltage. |
| R1 | Fixed resistor marked 100 Ω | Measure its actual resistance and find its power rating in the documentation for the exact part. |
| S2 | Momentary press switch | Confirm that its switch contacts are open when released and joined only while pressed. |

The part identities match the [Elenco Snap Circuits Home Learning Kit manual](https://www.elenco.com/wp-content/uploads/2015/03/2018_SCHL-1_Manual_LR.pdf). The manual specifies two 1.5 V AA cells for B1 and identifies R1 as 100 Ω, but the actual classroom cells and resistor must still be checked.

Complete this checklist before students begin:

- [ ] Remove power and inspect B1, R1, and S2 for damage, loose snaps, corrosion, or overheating.
- [ ] Record the installed AA cell type and its nominal voltage per cell: ____________________.
- [ ] Set up the meter for DC voltage and measure B1 with S2 released. Actual battery voltage: __________ V.
- [ ] Disconnect B1, isolate R1 from other paths, and measure it in resistance mode. Actual resistance: __________ Ω.
- [ ] Find and record R1's power rating from the manufacturer, supplier, or teacher inventory record. Power rating: __________ W. Do not approve the activity if the rating is unknown.
- [ ] Calculate expected current using the measured values: $I = V / R$. Expected current: __________ A = __________ mA.
- [ ] Calculate expected resistor power using $P = V \times I$ or $P = V^2 / R$. Expected power: __________ W. Confirm that it is below R1's verified power rating.
- [ ] Read the exact meter's manual and inspect its current fuse. Choose a current input and initial range rated above the expected current. Current jack: __________. Initial range: __________.
- [ ] Confirm that the expected voltage and current are within the ratings of B1, R1, S2, the leads, and the meter. If any rating is exceeded or unknown, stop and use different teacher-approved equipment.

For a nominal example only, two 1.5 V cells give 3.0 V. With an ideal 100 Ω resistor, $I = 3.0 / 100 = 0.030$ A, or 30 mA, and $P = 3.0^2 / 100 = 0.09$ W. Do not use those nominal values as the class results; calculate again from the actual battery voltage and measured resistance.

## Build and Trace the Circuit

In this lesson, an **open circuit** has a break in its path, so current cannot travel all the way around the loop. A **closed circuit** has one complete path, so current can flow. Current means the flow of electric charge.

Follow these steps with a teacher:

1. Remove both AA cells from B1. Keep S2 released while building.
2. Place B1, R1, and S2 on the Snap Circuits base grid to match [`sample_circuit/base_layout.jpg`](sample_circuit/base_layout.jpg).
3. Snap one end of the 100 Ω R1 resistor to the negative (`−`) snap on B1.
4. Snap the other end of R1 to either end of S2. R1 works in either direction.
5. Leave the free end of S2 and the positive (`+`) snap on B1 unconnected. Do not add a snap wire between them. This gap is intentional: the multimeter will bridge it later during the teacher-verified current measurement.
6. Ask the teacher to trace the unfinished path in this order: B1 negative → R1 → S2 → open gap. Do not reinstall the cells until the teacher confirms that R1 and S2 are connected in series and that the free end of S2 does not connect to B1 positive.

## Observe an Open and Closed Path

First practice tracing the path while B1 has no cells installed and the multimeter is not connected:

1. Release S2. The switch is open, and the intentional gap between S2 and B1 positive is also open. Current has no complete path.
2. Press and hold S2. This joins the two ends of the switch, but the circuit is still open at the intentional gap. Pressing S2 alone cannot make current flow.
3. Release S2. Keep the intentional gap open until the teacher-approved current procedure.

This circuit has no lamp, so do not look for light as evidence of current. Later, after you calculate a predicted current, the teacher-verified current procedure will place the multimeter in series between the free end of S2 and B1 positive. The meter then becomes part of the only complete path:

`B1 positive → multimeter → S2 → R1 → B1 negative`

The current reading will provide the evidence:

- With the meter safely in series and S2 released, the open switch should give a reading at or near 0 mA.
- While S2 is pressed, a nonzero reading shows that current is flowing through the closed B1/R1/S2 path and the meter.
- Releasing S2 should return the reading to at or near 0 mA.

Do not switch the meter to current mode or connect it to this circuit until the lesson reaches the teacher-verified current procedure.

## Why R1 Has Two Jobs

R1 is the circuit's **load**. A load is a part that uses electrical energy and limits how much current can flow. In this circuit, R1 turns a small amount of electrical energy into heat while its resistance keeps the current from becoming too large. R1 must stay in the path during the current measurement; never bypass it with the meter or a wire.

R1 is also a **fixed resistor**. Fixed means its resistance is designed to stay approximately the same instead of being adjusted like a knob. Its measured resistance may differ a little from the `100 Ω` label, so this lesson uses the measured value in the Ohm's law calculation. The same R1 that safely limits current therefore supplies the resistance value used to predict that current.

## Measure R1 Resistance First

**Resistance must be measured with all power removed. Never use resistance mode on a powered circuit.**

Resistance tells how strongly a part limits current. The meter marks resistance with the omega symbol (`Ω`). Measure R1 before measuring voltage or current:

1. Confirm that both AA cells are removed from B1. Show the empty holder to the teacher.
2. Keep the intentional gap between the free end of S2 and B1 positive open.
3. Unsnap the R1 end that connects to B1 negative. Leave that R1 terminal lifted so R1 has no complete path through B1 or S2. The [`sample_circuit/resistance.jpg`](sample_circuit/resistance.jpg) photo shows this isolated-resistor setup.
4. Turn the meter off while checking the leads. Put the black lead in **COM** and the red lead in the jack marked for voltage and resistance, usually **VΩ**. The red lead must not be in a current jack.
5. Turn the dial to resistance (`Ω`). For a manual-ranging meter, begin on a teacher-approved range above 100 Ω, then select a lower suitable range if the meter manual allows it. An auto-ranging meter chooses the range itself.
6. Ask the teacher to verify the empty B1 holder, lifted R1 terminal, lead jacks, and resistance setting before the probes touch R1.
7. Touch one probe tip to each metal terminal of R1. Probe color and direction do not matter for this resistor. Keep fingers off the metal probe tips and wait for a steady number.
8. Read both the number and its unit. Record the measured resistance as __________ Ω. If the meter displays kilo-ohms (`kΩ`), ask the teacher to help convert it to ohms before recording.
9. Remove the probes and turn the meter off. Snap R1 back onto B1 negative, but leave the free S2 terminal and B1 positive unconnected for the later current measurement. Ask the teacher to recheck the layout before any cells are installed.

The `100 Ω` printed on R1 is its labeled value. The measured value may be somewhat different, so use the measured value in the later Ohm's law prediction.

## Measure B1 Voltage Next

Voltage is the electrical push supplied by the battery. A voltage measurement is made **in parallel**, which means the two probes touch the two B1 battery terminals at the same time. Keep the intentional gap between S2 and B1 positive open during this measurement.

1. Confirm that R1 is snapped back onto B1 negative and that the free S2 terminal is still not connected to B1 positive.
2. Install the two teacher-approved AA cells in B1 in the directions marked on the holder. Do not press S2.
3. Turn the meter off while checking the leads. Put the black lead in **COM** and the red lead in the voltage jack, usually marked **VΩ**. The red lead must not be in a current jack.
4. Turn the dial to DC voltage, shown as `V` with a straight line. Do not use the AC voltage setting. For a manual-ranging meter, begin on a teacher-approved DC voltage range above B1's expected voltage. An auto-ranging meter chooses the range itself.
5. Ask the teacher to verify the lead jacks, DC voltage setting, installed cells, and open gap between S2 and B1 positive.
6. Touch the black probe directly to B1's negative (`−`) terminal and the red probe directly to B1's positive (`+`) terminal. The probes are now in parallel across B1. Keep the probe tips from touching each other.
7. Wait for a steady reading, then record B1's measured voltage: __________ V.
8. Remove both probes and turn the meter off. Remove both AA cells from B1 before changing the circuit or preparing for another meter mode. Leave the S2-to-B1 gap open.

## Predict Current with Ohm's Law

A **prediction** is what you expect a measurement to show before you test it. Ohm's law connects voltage (`V`), current (`I`), and resistance (`R`):

$$V = I \times R$$

To predict current, rearrange the formula:

$$I = \frac{V}{R}$$

Use B1's measured voltage—not just the voltage printed on the batteries—and R1's measured resistance—not just its `100 Ω` label. Keep the units consistent: volts divided by ohms gives current in amperes. Then multiply amperes by 1,000 to convert to milliamps (`mA`).

For example, `3.0 V ÷ 100 Ω = 0.030 A`, and `0.030 A × 1,000 = 30 mA`. This is only an example; use your own measured numbers below.

1. Confirm that the probes are removed, the meter is off, both AA cells are out of B1, and the S2-to-B1 gap is open.
2. Copy B1's measured voltage: __________ V.
3. Copy R1's measured resistance: __________ Ω.
4. Divide voltage by resistance: __________ V ÷ __________ Ω = __________ A.
5. Convert amperes to milliamps: __________ A × 1,000 = __________ mA.
6. Record your **predicted current**: __________ mA.
7. Ask the teacher to check the calculation. Do not move the red lead to a current jack, select current mode, or connect anything across the S2-to-B1 gap yet.

## Measure Current in Series — Teacher Verification Required

Current must be measured **in series**, which means the meter becomes one link in the circuit's only path. Begin this procedure only after the predicted current above has been recorded and checked by the teacher.

> **STOP:** A meter in current mode must never be connected directly across B1's positive and negative terminals or directly across any component. That could create a short circuit, damage the meter, or blow its fuse. In this procedure, the meter bridges only the intentional gap between B1 positive and the free S2 terminal, so R1 remains in the path.

1. Keep S2 released. Confirm that both AA cells are removed from B1 before changing the meter or circuit.
2. Show the teacher the recorded predicted current. The teacher must confirm that it is below the limit of the selected current jack and range on this exact meter.
3. With the meter off, keep the black lead in **COM**. Move the red lead from the voltage jack to the teacher-approved current jack identified during teacher preparation.
4. Set the dial to DC current. Start on the highest suitable safe current range that includes the predicted value. Follow the exact meter manual; never use a jack or range whose limit is below the predicted current.
5. Connect the red probe or teacher-approved lead attachment to B1 positive (`+`). Connect the black probe or attachment to the free S2 terminal. Do not connect the probes to B1 negative. The path is now `B1 positive → meter → S2 → R1 → B1 negative`.
6. **Teacher checkpoint:** Before restoring power, the teacher verifies all of the following:
   - R1 is snapped between B1 negative and S2;
   - the meter bridges only the free S2 terminal and B1 positive;
   - S2 is released;
   - the black lead is in COM and the red lead is in the approved current jack; and
   - the dial is on the approved DC current range.
7. Only after teacher approval, install the two AA cells in B1 in the marked directions. With S2 released, the reading should be at or near `0 mA` because the switch keeps the path open.
8. Press and hold S2 only long enough to get a steady reading. Record the measured current: __________ mA. Release S2 immediately after the reading.
9. Compare using the same unit: predicted current __________ mA; measured current __________ mA. Are they approximately close? __________. The values do not need to be exactly equal.
10. Remove both AA cells from B1 before disconnecting either meter lead or changing the dial. Confirm that B1 is empty.
11. Turn the meter off, disconnect it from the circuit, and return the red lead from the current jack to the voltage/resistance jack, usually marked **VΩ**. Leave the S2-to-B1 gap open.

## Troubleshooting — Stop and Check Safely

When a reading is unexpected, release S2, remove the probes, and remove both AA cells before changing connections, lead jacks, or meter mode. Ask the teacher to help.

| Problem | Safe check |
| --- | --- |
| The meter mode does not match the measurement. | With power removed and the meter off, choose resistance (`Ω`) for isolated R1, DC voltage for B1, or the teacher-approved DC current range for the series-current procedure. |
| A lead is in the wrong port. | Check that black is in **COM**. Red belongs in **VΩ** for resistance and voltage. Move red to a current jack only during the powered-down, teacher-verified current procedure, then return it to **VΩ** afterward. |
| B1 voltage has a minus sign. | Remove the probes. Put black on B1 negative (`−`) and red on B1 positive (`+`), then measure again in DC voltage mode. |
| The display shows `OL`, `1`, or another overload symbol. | Remove the probes and power. The range may be too low, or the resistance path may be open. The teacher checks the meter manual, chooses a safe higher range if appropriate, and checks probe contact before trying again. |
| Current stays near `0 mA`. | With S2 released, this is expected because the circuit is open. If it stays near zero while S2 is pressed, remove both cells. The teacher checks that the meter bridges only B1 positive and the free S2 terminal, that R1 remains in the path, and that all snaps and lead contacts are secure. |
| The teacher suspects a blown meter fuse. | Stop using the meter's current ranges and label the meter for teacher inspection. Only a qualified adult following the exact meter manual may inspect or replace its fuse. Never open the meter, bypass a fuse, or substitute a different fuse. |

## Student Instructions

1. Read the safety directions your teacher provides before touching the circuit or meter.
2. With the circuit unpowered, identify the B1 battery holder, 100 Ω R1 fixed resistor, and S2 press switch.
3. Follow **Build and Trace the Circuit** to assemble B1, R1, and S2 with no cells installed.
4. Follow **Observe an Open and Closed Path** to trace what S2 changes. Wait for the later teacher checkpoint before using current mode.
5. Read **Why R1 Has Two Jobs** and make sure R1 stays in the circuit path.
6. Follow **Measure R1 Resistance First** and record the measured value before any powered measurements.
7. Follow **Measure B1 Voltage Next** and record B1's voltage while the intentional S2-to-B1 gap remains open.
8. Follow **Predict Current with Ohm's Law** and record the predicted current in milliamps before changing the meter setup.
9. Follow **Measure Current in Series — Teacher Verification Required** only when directed. Stop at every teacher checkpoint.
10. Record each observation and measurement in [`recording-activity.md`](recording-activity.md).
11. Compare your predicted current with the later measured current.
12. Answer the reflection questions in your own words.

This lesson does not require an Arduino, programming, building firmware, or uploading firmware.
