#pragma once

#include <SDL_ttf.h>

#include "ui_core.h"

class UILabel : public UIElement
{
public:
    UILabel(int x, int y, TTF_Font *font, const std::string &text, uint32_t color) : UIElement(x, y, 0, 0), font(font), text(text), color(color)
    {
        TTF_SizeText(font, text.c_str(), &bounds.width, &bounds.height);
    }

    void update(InputManager *input) override {};
    void set_text(const std::string &new_text)
    {
        if (text != new_text)
        {
            text = new_text;
            TTF_SizeText(font, text.c_str(), &bounds.width, &bounds.height);
            mark_dirty();
        }
    }

    void draw(std::vector<uint32_t> &pixel_buffer, int buffer_width) override
    {
        if (text.empty() || font == nullptr)
            return;

        SDL_Color sdl_color;
        sdl_color.a = (color & 0xFF000000) >> 24;
        sdl_color.r = (color & 0x00FF0000) >> 16;
        sdl_color.g = (color & 0x0000FF00) >> 8;
        sdl_color.b = (color & 0x000000FF);

        SDL_Surface *text_surf = TTF_RenderText_Blended(font, text.c_str(), sdl_color);
        if (!text_surf)
            return;

        SDL_Surface *formatted_surf = SDL_ConvertSurfaceFormat(text_surf, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_FreeSurface(text_surf);

        if (!formatted_surf)
            return;

        uint32_t *src_pixels = (uint32_t *)formatted_surf->pixels;
        int buffer_height = pixel_buffer.size() / buffer_width;

        for (int y = 0; y < formatted_surf->h; ++y)
        {
            for (int x = 0; x < formatted_surf->w; ++x)
            {

                int dest_x = bounds.x + x;
                int dest_y = bounds.y + y;

                // Bounds check
                if (dest_x >= 0 && dest_x < buffer_width && dest_y >= 0 && dest_y < buffer_height)
                {

                    int dest_index = dest_y * buffer_width + dest_x;
                    int src_index = y * formatted_surf->w + x;

                    pixel_buffer[dest_index] = Utils::ColorUtils::blend_pixel(src_pixels[src_index], pixel_buffer[dest_index]);
                }
            }
        }

        SDL_FreeSurface(formatted_surf);
    }

private:
    TTF_Font *font;
    std::string text;
    uint32_t color;
};