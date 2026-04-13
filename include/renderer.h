#pragma once
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <cstdint>
#include <iostream>

#include "world.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void render();

    bool initialize(int width, int height, const World *world, SDL_Renderer *sdl_renderer_ptr);

    const uint32_t *get_pixel_buffer() const;

private:
    int width;
    int height;

    const World *world_ptr;

    std::vector<uint32_t> pixel_buffer;

    SDL_Texture *world_texture_ptr;

    SDL_Renderer *sdl_renderer_ptr;

    TTF_Font *debug_font;
};