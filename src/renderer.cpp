#include "renderer.h"

#include "engine.h"

Renderer::Renderer() : world_ptr(nullptr), world_texture_ptr(nullptr), ui_texture_ptr(nullptr) {}

bool Renderer::initialize(int width, int height, const World *world_ptr, SDL_Renderer *sdl_renderer_ptr)
{
    Renderer::width = width;
    Renderer::height = height;

    Renderer::world_ptr = world_ptr;

    Renderer::sdl_renderer_ptr = sdl_renderer_ptr;

    Renderer::pixel_buffer.reserve(width * height);

    world_texture_ptr = SDL_CreateTexture(
        sdl_renderer_ptr,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        world_ptr->get_width(), world_ptr->get_height());

    ui_texture_ptr = SDL_CreateTexture(
        sdl_renderer_ptr,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height);

    SDL_SetTextureBlendMode(ui_texture_ptr, SDL_BLENDMODE_BLEND);

    return true;
}

void Renderer::render()
{
    if (world_texture_ptr == nullptr)
        return;

    SDL_SetRenderDrawColor(sdl_renderer_ptr, 60, 60, 60, 255);
    SDL_RenderClear(sdl_renderer_ptr);

    SDL_Rect src_rect;

    if (world_ptr != nullptr)
    {
        const uint32_t *world_colors = world_ptr->get_cell_colors();

        SDL_UpdateTexture(
            world_texture_ptr,
            NULL,
            world_ptr->get_cell_colors(),
            world_ptr->get_width() * sizeof(uint32_t));

        SDL_RenderCopy(sdl_renderer_ptr, world_texture_ptr, NULL, NULL);
    }

    SDL_RenderPresent(sdl_renderer_ptr);
}

const uint32_t *Renderer::get_pixel_buffer() const { return pixel_buffer.data(); }