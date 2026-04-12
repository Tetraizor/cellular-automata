#include "utils/color_utils.h"

uint32_t Utils::ColorUtils::to_argb8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return (a << 24) + (r << 16) + (g << 8) + b;
}
