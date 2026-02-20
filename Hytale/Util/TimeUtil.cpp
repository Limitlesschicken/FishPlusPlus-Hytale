/*
 * Copyright (c) FishPlusPlus.
 */
#include "Util.h"

#include <cmath>
#include <cstdint>

SimpleTime Util::HoursToTime(float hours)
{
    if (hours < 0.0f) hours = 0.0f;
    if (hours >= 24.0f) hours = fmod(hours, 24.0f);

    SimpleTime t{};

    t.hour = static_cast<int>(std::floor(hours));
    float rem = hours - t.hour;

    t.minute = static_cast<int>(std::floor(rem * 60.0f));
    rem = rem * 60.0f - t.minute;

    t.second = static_cast<int>(std::floor(rem * 60.0f));
    rem = rem * 60.0f - t.second;

    t.millisecond = static_cast<int>(std::round(rem * 1000.0f));

    if (t.millisecond == 1000) {
        t.millisecond = 0;
        t.second++;
    }
    if (t.second == 60) {
        t.second = 0;
        t.minute++;
    }
    if (t.minute == 60) {
        t.minute = 0;
        t.hour = (t.hour + 1) % 24;
    }

    return t;
}

int64_t Util::BuildTicksFromHours(float hours)
{
    SimpleTime t = Util::HoursToTime(hours);

    const int64_t TicksPerSecond = 10'000'000;
    const int64_t TicksPerMillisecond = 10'000;

    return
        (int64_t)t.hour * 3600 * TicksPerSecond +
        (int64_t)t.minute * 60 * TicksPerSecond +
        (int64_t)t.second * TicksPerSecond +
        (int64_t)t.millisecond * TicksPerMillisecond;
}