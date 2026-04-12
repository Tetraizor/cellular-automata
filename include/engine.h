#pragma once

#include <cstdint>
#include "window_manager.h"
#include "renderer.h"

const uint64_t FIXED_TICK_RATE = 60; // Per second

class Engine
{
public:
    static Engine &get();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    int run();

    uint64_t get_ticks_passed() const { return ticks; };
    uint64_t get_millis_passed() const { return millis; };
    int get_current_fps() const { return current_fps; };

    static const int WORLD_WIDTH = 200;
    static const int WORLD_HEIGHT = 150;

    static const int ZOOM_FACTOR = 5;

private:
    Engine();
    ~Engine();

    uint64_t ticks;
    double millis;

    uint64_t target_fps = 144;
    int current_fps = target_fps;

    bool is_running;

    WindowManager wm;
    Renderer renderer;
};