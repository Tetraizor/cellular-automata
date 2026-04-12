#pragma once
#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>

#include "world.h"

class Renderer
{
public:
    Renderer();
    ~Renderer() = default;

    void render();
    void set_target_world(const World *world);

    bool initialize(int width, int height, SDL_Renderer *sdl_renderer_ptr);

    const uint32_t *get_pixel_buffer() const;

private:
    int width;
    int height;

    const World *world;

    std::vector<uint32_t> pixel_buffer;

    SDL_Texture *screen_texture_ptr;
    SDL_Renderer *sdl_renderer_ptr;
};