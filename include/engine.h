#pragma once

#include <cstdint>
#include "window_manager.h"
#include "renderer.h"
#include "input_manager.h"
#include "ui/ui_manager.h"

const uint64_t FIXED_TICK_RATE = 144; // Per second

class Engine
{
public:
    static Engine &get();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    int run(int width = 600, int height = 400, int zoom_factor = 2);
    void quit(int return_code = 0);

    uint64_t get_ticks_passed() const { return ticks; };
    uint64_t get_millis_passed() const { return millis; };
    int get_current_fps() const { return current_fps; };

    World *get_world() { return &world; }
    InputManager *get_input_manager() { return &input_manager; }
    Renderer *get_renderer() { return &renderer; }
    WindowManager *get_window_manager() { return &wm; }
    UIManager *get_ui_manager() { return &ui_manager; }

    int get_world_width() { return world_width; }
    int get_world_height() { return world_height; }
    int get_zoom_factor() { return zoom_factor; }

    uint8_t get_selected_material() const { return selected_material; }
    void set_selected_material(uint8_t m) { selected_material = m; }

private:
    Engine();
    ~Engine();

    uint64_t ticks;
    double millis;

    uint64_t target_fps = 144;
    int current_fps = target_fps;

    bool is_running;
    int return_code;

    int world_height;
    int world_width;
    int zoom_factor;
    uint8_t selected_material = 1; // defaults to SAND

    WindowManager wm;
    Renderer renderer;
    InputManager input_manager;
    World world;
    UIManager ui_manager;
};