#pragma once

#include <vector>
#include <cstdint>

#include "utils/color_utils.h"

struct Cell
{
    uint8_t id;
    uint8_t flags;
};

enum MaterialType : uint8_t
{
    EMPTY = 0,
    SAND = 1,
    STONE = 2,
    WATER = 3,
};

static const uint32_t mat_colors[256] = {
    Utils::ColorUtils::to_argb8888(0, 0, 0),
    Utils::ColorUtils::to_argb8888(194, 178, 128),
    Utils::ColorUtils::to_argb8888(100, 100, 100),
    Utils::ColorUtils::to_argb8888(50, 70, 255),
};

class World
{
public:
    World() = default;
    ~World() = default;

    void initialize(int width, int height);

    void clear();
    void update();
    const uint32_t *get_cell_colors() const;

    int get_width() const { return width; }
    int get_height() const { return height; }

    uint8_t get_cell_id(int x, int y) const;
    const Cell &get_cell(int x, int y) const;
    const Cell &get_cell(int index) const;

    void move_cell(int x1, int y1, int x2, int y2);

    void set_cell(int x, int y, uint8_t new_id);

    int coords_to_index(int x, int y) const;
    std::pair<int, int> index_to_coords(int index) const;

    uint32_t rng_state = 11111111;

private:
    int width;
    int height;
    std::vector<Cell> grid;
    std::vector<uint32_t> flat_cell_color_list;
};