#include "world.h"

#include <iostream>

#include "utils/color_utils.h"

int current_position = 0;

World::World(int width, int height) : width(width), height(height), grid(width * height), flat_cell_color_list(width * height)
{
    std::cout << "World is initialized with w: " << width << ", height: " << height << ", totaling to " << (width * height) << " cells." << std::endl;
}

void World::update()
{
    std::pair previous_coords = index_to_coords(current_position % (width * height));
    std::pair current_coords = index_to_coords((current_position + 1) % (width * height));

    set_cell(previous_coords.first, previous_coords.second, 0, Utils::ColorUtils::to_argb8888(0, 0, 0, 255));
    set_cell(current_coords.first, current_coords.second, 0, Utils::ColorUtils::to_argb8888(255, 0, 0, 255));

    current_position++;
}

const uint32_t *World::get_cell_colors() const
{
    return World::flat_cell_color_list.data();
}

void World::set_cell(int x, int y, uint8_t new_id, uint32_t new_color)
{
    int index = coords_to_index(x, y);

    grid[index].id = new_id;
    grid[index].color = new_color;

    World::flat_cell_color_list[index] = new_color;
}

int World::coords_to_index(int x, int y) const
{
    return (y * width) + x;
}

std::pair<int, int> World::index_to_coords(int index) const
{
    int x = index % width;
    int y = index / width;

    return std::pair(x, y);
}