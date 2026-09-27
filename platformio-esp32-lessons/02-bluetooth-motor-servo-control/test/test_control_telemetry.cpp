// Host check, from this lesson directory:
// g++ -std=c++11 -Wall -Wextra -Werror -Imain test/test_control_telemetry.cpp -o /tmp/lesson02-telemetry-check
// /tmp/lesson02-telemetry-check

#include "control_telemetry.h"

#include <assert.h>
#include <stdio.h>

int main() {
    control::ControlTelemetrySchedule schedule;

    assert(!schedule.periodicDue(0));
    assert(schedule.shouldEmit(0, true));
    schedule.markPeriodicEmitted(0);

    assert(!schedule.shouldEmit(199, false));
    assert(schedule.shouldEmit(199, true));
    assert(schedule.shouldEmit(200, false));

    unsigned int periodicRecords = 0;
    for (uint32_t now = 1; now <= 1000; ++now) {
        if (!schedule.shouldEmit(now, false)) continue;
        ++periodicRecords;
        schedule.markPeriodicEmitted(now);
    }
    assert(periodicRecords == 5);

    // Immediate transition records bypass the periodic gate without shifting it.
    assert(schedule.shouldEmit(1001, true));
    assert(!schedule.shouldEmit(1199, false));
    assert(schedule.shouldEmit(1200, false));

    // The writer never emits a partial application record when the shared
    // BTstack console temporarily has less capacity than the queued line.
    assert(control::telemetryWriteSize(0, 300) == 0);
    assert(control::telemetryWriteSize(127, 300) == 0);
    assert(control::telemetryWriteSize(299, 300) == 0);
    assert(control::telemetryWriteSize(300, 300) == 300);
    assert(control::telemetryWriteSize(4096, 300) == 300);

    puts("Telemetry checks passed: transitions, five-per-second limit, and constrained console capacity.");
}
