#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "renderer.h"

class WindowManager
{
public:
    WindowManager() = default;
    ~WindowManager();

    bool initialize();
    void set_pixel_buffer(std::vector<uint32_t> pixel_buffer);

    int get_scaled_window_width() const { return unscaled_window_width * zoom_factor; }
    int get_scaled_window_height() const { return unscaled_window_height * zoom_factor; }
    int get_zoom_factor() const { return zoom_factor; }

    UIRect get_world_rect() const { return world_rect; }
    UIRect get_top_panel_rect() const { return top_panel_rect; }
    UIRect get_bottom_panel_rect() const { return bottom_panel_rect; }
    UIRect get_right_panel_rect() const { return right_panel_rect; }

    SDL_Renderer *get_sdl_renderer_ptr() const { return sdl_renderer_ptr; }
    SDL_Window *get_sdl_window_ptr() const { return sdl_window_ptr; }

private:
    int unscaled_window_width;
    int unscaled_window_height;
    int zoom_factor;

    UIRect world_rect;
    UIRect right_panel_rect;
    UIRect top_panel_rect;
    UIRect bottom_panel_rect;

    SDL_Window *sdl_window_ptr = nullptr;
    SDL_Renderer *sdl_renderer_ptr = nullptr;
};