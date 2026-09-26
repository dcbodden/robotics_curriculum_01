// Host check, from this lesson directory:
// g++ -std=c++11 -Wall -Wextra -Werror -Imain test/test_control_policy.cpp -o /tmp/lesson02-policy-check
// /tmp/lesson02-policy-check

#include "control_policy.h"

#include <assert.h>
#include <stdio.h>

namespace {

using control::ControlPolicy;
using control::ControllerReport;
using control::ControlState;
using control::TransitionReason;

ControllerReport report(int controllerIndex,
                        uint32_t receivedAtMs,
                        int axisX = 0,
                        int axisY = 0,
                        bool optionsPressed = false,
                        bool circlePressed = false,
                        bool validGamepad = true) {
    return {controllerIndex, receivedAtMs, axisX, axisY, validGamepad, optionsPressed, circlePressed};
}

void armFromNeutral(ControlPolicy& policy, int controllerIndex, uint32_t neutralAtMs) {
    assert(policy.submitReport(report(controllerIndex, neutralAtMs)));
    assert(policy.neutralReady());
    assert(policy.submitReport(report(controllerIndex, neutralAtMs + 1, 0, 0, true)));
    assert(policy.armed());
}

void checkControllerSelection() {
    ControlPolicy policy;
    assert(policy.activeController() == control::kNoController);
    assert(!policy.controllerConnected(7, false));
    assert(policy.controllerConnected(1, true));
    assert(policy.activeController() == 1);
    assert(policy.state() == ControlState::Disarmed);
    assert(!policy.controllerConnected(2, true));
    assert(!policy.submitReport(report(2, 10)));
    policy.controllerDisconnected(2);
    assert(policy.activeController() == 1);

    policy.controllerDisconnected(1);
    assert(policy.activeController() == control::kNoController);
    assert(policy.state() == ControlState::Disconnected);
    assert(policy.lastTransition() == TransitionReason::ControllerDisconnected);

    // Controller 2 was already present but is not promoted silently.
    assert(!policy.submitReport(report(2, 20)));
    assert(policy.activeController() == control::kNoController);
    assert(policy.controllerConnected(2, true));
    assert(policy.activeController() == 2);
}

void checkArmingAndDisarming() {
    ControlPolicy policy;
    assert(!policy.neutralReady());
    assert(!policy.armed());
    assert(policy.controllerConnected(0, true));

    // Displaced controls cannot establish neutral readiness or arm.
    assert(policy.submitReport(report(0, 0, 41, 0, true)));
    assert(!policy.neutralReady());
    assert(!policy.armed());

    // The first neutral report observes readiness but cannot arm from the same
    // OPTIONS edge. A held button is not another edge.
    assert(policy.submitReport(report(0, 1, 0, 0, true)));
    assert(policy.neutralReady());
    assert(!policy.armed());
    assert(policy.submitReport(report(0, 2, 0, 0, true)));
    assert(!policy.armed());
    assert(policy.submitReport(report(0, 3)));
    assert(policy.submitReport(report(0, 4, 0, 0, true)));
    assert(policy.armed());
    assert(policy.lastTransition() == TransitionReason::Armed);

    assert(policy.submitReport(report(0, 5, 200, -300)));
    assert(policy.output().targetMotor > 0);
    assert(policy.output().servoPulseUs > control::kServoCenterPulseUs);

    assert(policy.submitReport(report(0, 6, 200, -300, false, true)));
    assert(policy.state() == ControlState::Disarmed);
    assert(!policy.neutralReady());
    assert(policy.output().targetMotor == 0);
    assert(policy.output().appliedMotor == 0);
    assert(policy.output().servoPulseUs == control::kServoCenterPulseUs);

    // Holding CIRCLE cannot rebuild readiness or permit an arm.
    assert(policy.submitReport(report(0, 7, 0, 0, true, true)));
    assert(!policy.neutralReady());
    assert(!policy.armed());
    assert(policy.submitReport(report(0, 8)));
    assert(policy.neutralReady());
    assert(!policy.armed());
    assert(policy.submitReport(report(0, 9, 0, 0, true)));
    assert(policy.armed());
}

void checkInvalidWatchdogAndReconnect() {
    ControlPolicy policy;
    assert(policy.controllerConnected(3, true));
    armFromNeutral(policy, 3, 100);
    assert(policy.submitReport(report(3, 102, 0, -512)));
    policy.update(121);
    assert(policy.output().appliedMotor == 16);

    assert(policy.submitReport(report(3, 122, 0, 0, false, false, false)));
    assert(policy.state() == ControlState::Failsafe);
    assert(policy.lastTransition() == TransitionReason::InvalidController);
    assert(!policy.neutralReady());
    assert(policy.output().appliedMotor == 0);
    assert(policy.output().servoPulseUs == control::kServoCenterPulseUs);

    // A new neutral report and later arm edge recover from invalid input.
    assert(policy.submitReport(report(3, 123)));
    assert(policy.neutralReady());
    assert(policy.submitReport(report(3, 124, 0, 0, true)));
    assert(policy.armed());
    assert(policy.submitReport(report(3, 125, 0, -512)));
    assert(policy.freshnessAgeMs(424) == 299);
    policy.update(424);
    assert(policy.armed());
    policy.update(425);
    assert(policy.state() == ControlState::Failsafe);
    assert(policy.lastTransition() == TransitionReason::WatchdogExpired);
    assert(policy.output().targetMotor == 0);
    assert(policy.output().appliedMotor == 0);
    assert(policy.output().servoPulseUs == control::kServoCenterPulseUs);
    assert(!policy.hasFreshReport());
    assert(policy.freshnessAgeMs(425) == UINT32_MAX);

    assert(policy.submitReport(report(3, 426)));
    assert(policy.neutralReady());
    assert(!policy.armed());
    assert(policy.submitReport(report(3, 427, 0, 0, true)));
    assert(policy.armed());

    policy.controllerDisconnected(3);
    assert(policy.state() == ControlState::Disconnected);
    assert(policy.output().appliedMotor == 0);
    assert(policy.controllerConnected(3, true));
    assert(policy.submitReport(report(3, 500, 0, -512, true)));
    assert(!policy.armed());
    assert(!policy.neutralReady());
    assert(policy.submitReport(report(3, 501)));
    assert(policy.neutralReady());
    assert(policy.submitReport(report(3, 502, 0, 0, true)));
    assert(policy.armed());
}

void checkSlewAndReversal() {
    ControlPolicy policy;
    assert(policy.controllerConnected(0, true));
    armFromNeutral(policy, 0, 0);
    assert(policy.submitReport(report(0, 2, 0, -512)));

    policy.update(20);
    assert(policy.output().appliedMotor == 0);
    policy.update(21);
    assert(policy.output().appliedMotor == 16);
    policy.update(41);
    assert(policy.output().appliedMotor == 32);
    for (uint32_t now = 61; now <= 381; now += 20) {
        if (now == 201 || now == 301) assert(policy.submitReport(report(0, now, 0, -512)));
        policy.update(now);
    }
    assert(policy.output().appliedMotor == 255);

    // A requested sign change drops immediately to zero. The next scheduled
    // update is entirely zero; only the following update may ramp opposite.
    assert(policy.submitReport(report(0, 382, 0, 511)));
    assert(policy.output().targetMotor == -255);
    assert(policy.output().appliedMotor == 0);
    assert(policy.output().reversalInterlock);
    policy.update(401);
    assert(policy.output().appliedMotor == 0);
    assert(policy.output().reversalInterlock);
    policy.update(402);
    assert(policy.output().appliedMotor == 0);
    assert(!policy.output().reversalInterlock);
    policy.update(422);
    assert(policy.output().appliedMotor == -16);

    // Normal zero requests slew, while explicit safety transitions bypass it.
    assert(policy.submitReport(report(0, 423)));
    assert(policy.output().appliedMotor == -16);
    policy.update(442);
    assert(policy.output().appliedMotor == 0);
    assert(policy.submitReport(report(0, 444, 0, -512)));
    policy.update(463);
    policy.update(464);
    assert(policy.output().appliedMotor == 16);
    assert(policy.submitReport(report(0, 465, 0, -512, false, true)));
    assert(policy.output().appliedMotor == 0);
}

}  // namespace

int main() {
    checkControllerSelection();
    checkArmingAndDisarming();
    checkInvalidWatchdogAndReconnect();
    checkSlewAndReversal();
    puts("Policy checks passed: selection, arming, failsafe, watchdog, slew, and reversal.");
}
