#pragma once

#include <string>
#include <functional>
#include <SDL_ttf.h>

#include "ui_core.h"
#include "utils/draw_utils.h"
#include "utils/color_utils.h"
#include "input_manager.h"

class UIButton : public UIElement
{
public:
    UIButton(int x, int y, int w, int h, int zoom,
             TTF_Font *font, const std::string &label, uint32_t swatch_color,
             std::function<void()> on_click, std::function<bool()> is_selected_fn)
        : UIElement(x, y, w, h), zoom(zoom), font(font), label(label),
          swatch_color(swatch_color), on_click(std::move(on_click)),
          is_selected_fn(std::move(is_selected_fn)) {}

    void update(InputManager *input) override
    {
        bool currently_selected = is_selected_fn();
        if (currently_selected != was_selected)
        {
            was_selected = currently_selected;
            mark_dirty();
        }

        bool is_down = input->get_is_cursor_down();
        if (is_down && !prev_down)
        {
            int mx = input->get_cursor_x() * zoom;
            int my = input->get_cursor_y() * zoom;
            if (mx >= bounds.x && mx < bounds.x + bounds.width &&
                my >= bounds.y && my < bounds.y + bounds.height)
            {
                on_click();
                mark_dirty();
            }
        }
        prev_down = is_down;
    }

    void draw(std::vector<uint32_t> &pixel_buffer, int buffer_width) override
    {
        int buffer_height = pixel_buffer.size() / buffer_width;
        bool selected = is_selected_fn();

        uint32_t bg = selected ? 0xFF505070 : 0xFF383838;
        Utils::DrawUtils::draw_fill_rect(pixel_buffer, buffer_width, buffer_height, bounds, bg);

        int pad = 6;
        int swatch_size = bounds.height - pad * 2;
        UIRect swatch_rect = {bounds.x + pad, bounds.y + pad, swatch_size, swatch_size};
        Utils::DrawUtils::draw_fill_rect(pixel_buffer, buffer_width, buffer_height, swatch_rect, swatch_color);

        if (!font)
            return;

        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface *surf = TTF_RenderText_Blended(font, label.c_str(), white);
        if (!surf)
            return;

        SDL_Surface *fmt = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_FreeSurface(surf);
        if (!fmt)
            return;

        uint32_t *src = (uint32_t *)fmt->pixels;
        int text_x = bounds.x + pad + swatch_size + pad;
        int text_y = bounds.y + (bounds.height - fmt->h) / 2;

        for (int y = 0; y < fmt->h; ++y)
        {
            for (int x = 0; x < fmt->w; ++x)
            {
                int dx = text_x + x;
                int dy = text_y + y;
                if (dx >= 0 && dx < buffer_width && dy >= 0 && dy < buffer_height)
                    pixel_buffer[dy * buffer_width + dx] = Utils::ColorUtils::blend_pixel(
                        src[y * fmt->w + x], pixel_buffer[dy * buffer_width + dx]);
            }
        }
        SDL_FreeSurface(fmt);
    }

private:
    int zoom;
    TTF_Font *font;
    std::string label;
    uint32_t swatch_color;
    std::function<void()> on_click;
    std::function<bool()> is_selected_fn;
    bool prev_down = false;
    bool was_selected = false;
};
