#pragma once

#include <cstdint>
#include "window_manager.h"
#include "renderer.h"
#include "input_manager.h"

const uint64_t FIXED_TICK_RATE = 144; // Per second

class Engine
{
public:
    static Engine &get();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    int run();
    void quit(int return_code = 0);

    uint64_t get_ticks_passed() const { return ticks; };
    uint64_t get_millis_passed() const { return millis; };
    int get_current_fps() const { return current_fps; };

    World *get_world() { return &world; }
    InputManager *get_input_manager() { return &input_manager; }
    Renderer *get_renderer() { return &renderer; }
    WindowManager *get_window_manager() { return &wm; }

    static const int WORLD_WIDTH = 600;
    static const int WORLD_HEIGHT = 400;

    static const int ZOOM_FACTOR = 3;

private:
    Engine();
    ~Engine();

    uint64_t ticks;
    double millis;

    uint64_t target_fps = 144;
    int current_fps = target_fps;

    bool is_running;
    int return_code;

    WindowManager wm;
    Renderer renderer;
    InputManager input_manager;
    World world;
};