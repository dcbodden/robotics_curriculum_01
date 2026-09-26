// Host check, from this lesson directory:
// g++ -std=c++11 -Wall -Wextra -Werror -Imain test/test_actuator_commands.cpp -o /tmp/lesson02-actuator-check
// /tmp/lesson02-actuator-check

#include "bridge_command.h"
#include "control_policy.h"

#include <assert.h>
#include <stdio.h>

namespace {

void assertBridge(int command, uint8_t expectedAin1, uint8_t expectedAin2) {
    const control::BridgeCommand bridge = control::bridgeCommandFromMotor(command);
    assert(bridge.ain1Duty == expectedAin1);
    assert(bridge.ain2Duty == expectedAin2);
    assert(bridge.ain1Duty == 0 || bridge.ain2Duty == 0);
}

void checkBridgeTruthTable() {
    assertBridge(1000, 255, 0);
    assertBridge(255, 255, 0);
    assertBridge(37, 37, 0);
    assertBridge(0, 0, 0);
    assertBridge(-37, 0, 37);
    assertBridge(-255, 0, 255);
    assertBridge(-1000, 0, 255);
}

void checkServoDutyConversion() {
    using namespace control;

    assert(clampServoPulseUs(0) == kServoMinimumPulseUs);
    assert(clampServoPulseUs(kServoCenterPulseUs) == kServoCenterPulseUs);
    assert(clampServoPulseUs(UINT16_MAX) == kServoMaximumPulseUs);

    const uint32_t minimumDuty = servoDutyFromPulseUs(kServoMinimumPulseUs);
    const uint32_t centerDuty = servoDutyFromPulseUs(kServoCenterPulseUs);
    const uint32_t maximumDuty = servoDutyFromPulseUs(kServoMaximumPulseUs);
    assert(minimumDuty < centerDuty);
    assert(centerDuty < maximumDuty);
    assert(servoDutyFromPulseUs(0) == minimumDuty);
    assert(servoDutyFromPulseUs(UINT16_MAX) == maximumDuty);
    assert(maximumDuty <= kServoPwmMaximumDuty);
}

void checkSafetyOutputsDeEnergizeBridge() {
    using namespace control;

    const ControlOutput startup = {0, 0, kServoCenterPulseUs, false};
    const ControlOutput disarmed = {0, 0, kServoCenterPulseUs, false};
    const ControlOutput failsafe = {0, 0, kServoCenterPulseUs, false};
    const ControlOutput reversing = {-255, 0, kServoCenterPulseUs, true};

    const ControlOutput safetyOutputs[] = {startup, disarmed, failsafe, reversing};
    for (const ControlOutput& output : safetyOutputs) {
        const int enforcedCommand = output.reversalInterlock ? 0 : output.appliedMotor;
        const BridgeCommand bridge = bridgeCommandFromMotor(enforcedCommand);
        assert(bridge.ain1Duty == 0);
        assert(bridge.ain2Duty == 0);
    }
}

}  // namespace

int main() {
    checkBridgeTruthTable();
    checkServoDutyConversion();
    checkSafetyOutputsDeEnergizeBridge();
    puts("Actuator checks passed: bridge truth table, safe-zero states, and servo duty bounds.");
}
