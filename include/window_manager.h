#pragma once

#include <SDL2/SDL.h>
#include <vector>

class WindowManager
{
public:
    WindowManager() = default;
    ~WindowManager();

    bool initialize(int width, int height);
    void set_pixel_buffer(std::vector<uint32_t> pixel_buffer);
    void set_target_refresh_rate(int refresh_rate);

private:
    int width;
    int height;

    int target_refresh_rate = 60;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};