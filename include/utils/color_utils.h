#pragma once

#include <cstdint>

namespace Utils::ColorUtils
{
    inline uint32_t to_argb8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
    {
        return (a << 24) + (r << 16) + (g << 8) + b;
    }

    inline uint32_t blend_pixel(uint32_t src, uint32_t dst)
    {
        uint32_t src_a = (src >> 24) & 0xFF;
        if (src_a == 0)
            return dst;
        if (src_a == 255)
            return src;

        uint32_t src_r = (src >> 16) & 0xFF;
        uint32_t src_g = (src >> 8) & 0xFF;
        uint32_t src_b = src & 0xFF;

        uint32_t dst_r = (dst >> 16) & 0xFF;
        uint32_t dst_g = (dst >> 8) & 0xFF;
        uint32_t dst_b = dst & 0xFF;

        uint32_t out_r = (src_r * src_a + dst_r * (255 - src_a)) / 255;
        uint32_t out_g = (src_g * src_a + dst_g * (255 - src_a)) / 255;
        uint32_t out_b = (src_b * src_a + dst_b * (255 - src_a)) / 255;

        return (0xFF << 24) | (out_r << 16) | (out_g << 8) | out_b;
    }
}