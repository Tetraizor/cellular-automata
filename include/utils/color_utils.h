#pragma once

#include <cstdint>

namespace Utils::ColorUtils
{
    uint32_t to_argb8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255U);
}