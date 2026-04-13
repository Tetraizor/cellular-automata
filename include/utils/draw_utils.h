#pragma once

#include <vector>
#include <cstdint>

#include "ui/ui_core.h"

namespace Utils::DrawUtils
{
    void draw_fill_rect(std::vector<uint32_t> &buffer, int buffer_width, int buffer_height, const UIRect &rect, uint32_t color)
    {
        int start_x = std::max(0, rect.x);
        int start_y = std::max(0, rect.y);

        int end_x = std::min(buffer_width, rect.x + rect.width);
        int end_y = std::min(buffer_height, rect.y + rect.height);

        for (int y = start_y; y < end_y; ++y)
        {
            for (int x = start_x; x < end_x; ++x)
            {
                buffer[y * buffer_width + x] = color;
            }
        }
    }
}