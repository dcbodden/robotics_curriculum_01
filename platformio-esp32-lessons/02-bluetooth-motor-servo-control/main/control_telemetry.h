// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "control_config.h"
#include "control_policy.h"

namespace control {

inline uint32_t telemetryElapsed(uint32_t nowMs, uint32_t thenMs) { return nowMs - thenMs; }

class ControlTelemetrySchedule {
public:
    bool shouldEmit(uint32_t nowMs, bool significantTransition) const {
        return significantTransition || periodicDue(nowMs);
    }

    bool periodicDue(uint32_t nowMs) const {
        return started_ && telemetryElapsed(nowMs, lastPeriodicAtMs_) >= kControlTelemetryIntervalMs;
    }

    void markPeriodicEmitted(uint32_t nowMs) {
        started_ = true;
        lastPeriodicAtMs_ = nowMs;
    }

private:
    bool started_ = false;
    uint32_t lastPeriodicAtMs_ = 0;
};

struct ControlTelemetrySnapshot {
    uint32_t elapsedMs;
    int activeController;
    ControlState state;
    uint32_t freshnessAgeMs;
    int axisX;
    int axisY;
    ControlOutput output;
    TransitionReason transitionReason;
    bool actuatorReady;
};

class ControlTelemetryWriter {
public:
    bool enqueue(const ControlTelemetrySnapshot& snapshot, bool significantTransition);
    void service();

private:
    enum : size_t {
        kQueueCapacity = 4,
        kLineCapacity = 512,
    };

    struct LineSlot {
        char bytes[kLineCapacity];
        uint16_t length = 0;
        uint16_t offset = 0;
    };

    LineSlot queue_[kQueueCapacity]{};
    uint8_t head_ = 0;
    uint8_t count_ = 0;
};

}  // namespace control
