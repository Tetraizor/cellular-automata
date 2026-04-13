#pragma once

#include <algorithm>

#include "ui_core.h"
#include "utils/draw_utils.h"

class UIPanel : public UIElement
{
public:
    UIPanel(int x, int y, int w, int h, uint32_t color) : UIElement(x, y, w, h), bg_color(color) {}

    void update(InputManager *input) override {}

    void draw(std::vector<uint32_t> &pixel_buffer, int buffer_width) override
    {
        int buffer_height = pixel_buffer.size() / buffer_width;

        Utils::DrawUtils::draw_fill_rect(pixel_buffer, buffer_width, buffer_height, bounds, bg_color);
    }

private:
    uint32_t bg_color;
};