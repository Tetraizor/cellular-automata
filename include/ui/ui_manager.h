#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <SDL2/SDL.h>

#include "ui_core.h"

class UIManager
{
public:
    UIManager() = default;
    ~UIManager()
    {
        SDL_DestroyTexture(ui_texture);
    }

    void initialize(int w, int h, SDL_Renderer *renderer)
    {
        width = w;
        height = h;

        pixel_buffer.resize(width * height, 0x00000000);

        ui_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            width, height);

        SDL_SetTextureBlendMode(ui_texture, SDL_BLENDMODE_BLEND);
    }

    void add_element(std::unique_ptr<UIElement> element)
    {
        elements.push_back(std::move(element));
    }

    void update(InputManager *input)
    {
        for (auto &element : elements)
        {
            element->update(input);
        }
    }

    void render(SDL_Renderer *renderer)
    {
        bool needs_redraw = false;

        for (const auto &element : elements)
        {
            if (element->is_dirty)
            {
                needs_redraw = true;
                break;
            }
        }

        if (needs_redraw)
        {
            std::fill(pixel_buffer.begin(), pixel_buffer.end(), 0x00000000);

            for (auto &element : elements)
            {
                element->draw(pixel_buffer, width);
                element->is_dirty = false;
            }

            SDL_UpdateTexture(ui_texture, NULL, pixel_buffer.data(), width * sizeof(uint32_t));
        }

        SDL_RenderCopy(renderer, ui_texture, NULL, NULL);
    }

private:
    int width, height;
    std::vector<uint32_t> pixel_buffer;
    SDL_Texture *ui_texture;
    std::vector<std::unique_ptr<UIElement>> elements;
};
