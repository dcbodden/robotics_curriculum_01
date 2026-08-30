# Lesson 01: Multimeter and Ohm's Law

In this standalone, non-code lesson, you will use a digital multimeter with a low-voltage circuit. You will observe what happens when a circuit path is open or closed, practice measuring electrical quantities, and compare measured current with a value calculated using Ohm's law.

## Learning Goals

By the end of the lesson, you will be able to:

- describe the difference between an open circuit and a closed circuit;
- recognize voltage, resistance, and current as different measurements;
- use voltage and resistance to calculate current; and
- compare a calculated result with a meter reading.

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

## Student Instructions

1. Read the safety directions your teacher provides before touching the circuit or meter.
2. With the circuit unpowered, identify the B1 battery holder, 100 Ω R1 fixed resistor, and S2 press switch.
3. Follow **Build and Trace the Circuit** to assemble B1, R1, and S2 with no cells installed.
4. Follow **Observe an Open and Closed Path** to trace what S2 changes. Wait for the later teacher checkpoint before using current mode.
5. Follow **Measure R1 Resistance First** and record the measured value before any powered measurements.
6. Make each voltage and current measurement only when directed. Stop at every teacher checkpoint.
7. Record each observation and measurement in [`recording-activity.md`](recording-activity.md).
8. Use your recorded voltage and resistance to calculate current, then compare it with the measured current.
9. Answer the reflection questions in your own words.

This lesson does not require an Arduino, programming, building firmware, or uploading firmware.
