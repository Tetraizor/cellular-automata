#pragma once

#include <cstdint>
#include "window_manager.h"

const uint64_t FIXED_TICK_RATE = 60; // Per second

class Engine
{
public:
    Engine();
    ~Engine();

    int run();

    uint64_t get_ticks_passed() const { return ticks; };
    uint64_t get_millis_passed() const { return millis; };

private:
    uint64_t ticks;
    double millis;

    bool is_running;

    WindowManager wm;
};