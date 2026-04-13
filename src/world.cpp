#include "world.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>

#include "utils/color_utils.h"
#include "physics.h"
#include "engine.h"

void World::initialize(int width, int height)
{
    World::width = width;
    World::height = height;
    World::grid.resize(width * height);
    World::flat_cell_color_list.resize(width * height);

    std::cout << "World is initialized with w: " << width << ", height: " << height << ", totaling to " << (width * height) << " cells." << std::endl;

    for (int x = 30; x < 150; x++)
    {
        int height = 3 + (rand() % 10);

        for (int y = 20; y >= 20 - height; y--)
        {
            set_cell(x, y, MaterialType::SAND);
        }
    }
}

void World::clear()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            set_cell(x, y, MaterialType::EMPTY);
        }
    }
}

inline void update_cell(World &world, int x, int y)
{
    int index = world.coords_to_index(x, y);

    const Cell &cell = world.get_cell(index);

    if (cell.id == MaterialType::EMPTY)
        return;

    switch (cell.id)
    {
    case MaterialType::SAND:
        Physics::update_powder(world, x, y);
        break;

    case MaterialType::WATER:
        Physics::update_liquid(world, x, y);
        break;

    case MaterialType::STONE:
        break;
    }
}

void World::update()
{
    InputManager *im = Engine::get().get_input_manager();

    if (im->get_is_cursor_down())
    {
        int cursor_x = im->get_cursor_x();
        int cursor_y = im->get_cursor_y();

        int brush_radius = 24;

        for (int y = cursor_y - brush_radius; y <= cursor_y + brush_radius; y++)
        {
            for (int x = cursor_x - brush_radius; x <= cursor_x + brush_radius; x++)
            {
                double xx = x - cursor_x;
                double yy = y - cursor_y;

                double distance = std::sqrt((xx * xx) + (yy * yy));

                if (grid[coords_to_index(x, y)].id == MaterialType::EMPTY && distance <= brush_radius)
                    set_cell(x, y, MaterialType::SAND);
            }
        }
    }

    int frame_count = Engine::get().get_ticks_passed();

    for (int y = height - 2; y >= 0; y--)
    {
        if (frame_count % 2 == 0)
        {
            for (int x = 0; x < width; x++)
            {
                update_cell(*this, x, y);
            }
        }
        else
        {
            for (int x = width - 1; x >= 0; x--)
            {
                update_cell(*this, x, y);
            }
        }
    }
}

const uint32_t *World::get_cell_colors() const
{
    return World::flat_cell_color_list.data();
}

void World::set_cell(int x, int y, uint8_t new_id)
{
    int index = coords_to_index(x, y);
    uint32_t new_color = mat_colors[new_id];

    grid[index].id = new_id;

    World::flat_cell_color_list[index] = new_color;
}

int World::coords_to_index(int x, int y) const
{
    return (std::clamp(y, 0, height - 1) * width) + std::clamp(x, 0, width - 1);
}

std::pair<int, int> World::index_to_coords(int index) const
{
    int x = index % width;
    int y = index / width;

    return std::pair(x, y);
}

uint8_t World::get_cell_id(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return MaterialType::STONE;

    return grid[coords_to_index(x, y)].id;
}

void World::move_cell(int x1, int y1, int x2, int y2)
{
    uint8_t id = get_cell_id(x1, y1);
    set_cell(x1, y1, MaterialType::EMPTY);
    set_cell(x2, y2, id);
}

const Cell &World::get_cell(int x, int y) const
{
    return grid[coords_to_index(x, y)];
}

const Cell &World::get_cell(int index) const
{
    return grid[index];
}