#include "renderer.h"

Renderer::Renderer() : world(nullptr), screen_texture_ptr(nullptr) {}

bool Renderer::initialize(int width, int height, SDL_Renderer *sdl_renderer_ptr)
{
    Renderer::width = width;
    Renderer::height = height;

    Renderer::sdl_renderer_ptr = sdl_renderer_ptr;

    Renderer::pixel_buffer.reserve(width * height);

    screen_texture_ptr = SDL_CreateTexture(
        sdl_renderer_ptr,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height);

    return true;
}

void Renderer::render()
{
    if (screen_texture_ptr == nullptr)
        return;

    SDL_SetRenderDrawColor(sdl_renderer_ptr, 24, 24, 24, 255);
    SDL_RenderClear(sdl_renderer_ptr);

    if (world != nullptr)
    {
        const uint32_t *world_colors = world->get_cell_colors();

        SDL_UpdateTexture(
            screen_texture_ptr,
            NULL,
            world->get_cell_colors(),
            width * sizeof(uint32_t));

        SDL_RenderCopy(sdl_renderer_ptr, screen_texture_ptr, NULL, NULL);
    }

    SDL_RenderPresent(sdl_renderer_ptr);
}

void Renderer::set_target_world(const World *world)
{
    Renderer::world = world;
}

const uint32_t *Renderer::get_pixel_buffer() const { return pixel_buffer.data(); }