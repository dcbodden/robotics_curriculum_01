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
| S2 | Momentary press switch | Confirm that the path is open when released and closed only while pressed. |

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

## Student Instructions

1. Read the safety directions your teacher provides before touching the circuit or meter.
2. With the circuit unpowered, identify the B1 battery holder, 100 Ω R1 fixed resistor, and S2 press switch.
3. Follow the teacher-approved directions to assemble and observe the circuit.
4. Make each voltage, resistance, and current measurement only when directed. Stop at every teacher checkpoint.
5. Record each observation and measurement in [`recording-activity.md`](recording-activity.md).
6. Use your recorded voltage and resistance to calculate current, then compare it with the measured current.
7. Answer the reflection questions in your own words.

This lesson does not require an Arduino, programming, building firmware, or uploading firmware.
