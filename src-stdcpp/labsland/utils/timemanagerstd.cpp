/*
 * Copyright (C) 2023 onwards LabsLand, Inc.
 * All rights reserved.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 */
#include <thread>
#include <chrono>

#include "timemanagerstd.h"

using namespace LabsLand::Utils;

void TimeManagerStd::sleepMs(uint32_t ms) {
    std::this_thread::sleep_for (std::chrono::milliseconds(ms));
}

void TimeManagerStd::sleepUs(uint32_t us) {
    std::this_thread::sleep_for (std::chrono::microseconds(us));
}

LabsLand::Utils::clock_t TimeManagerStd::getAbsoluteTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

uint64_t TimeManagerStd::getClocksPerSec() {
    return CLOCKS_PER_SEC;
}

