#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "renderer.h"

class WindowManager
{
public:
    WindowManager() = default;
    ~WindowManager();

    bool initialize(int width, int height);
    void set_pixel_buffer(std::vector<uint32_t> pixel_buffer);
    void set_target_refresh_rate(int refresh_rate);

    SDL_Renderer *get_sdl_renderer_ptr() const { return sdl_renderer_ptr; }
    SDL_Window *get_sdl_window_ptr() const { return sdl_window_ptr; }

private:
    int width;
    int height;

    int target_refresh_rate = 60;

    SDL_Window *sdl_window_ptr = nullptr;
    SDL_Renderer *sdl_renderer_ptr = nullptr;
};