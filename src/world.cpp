#include "world.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <chrono>

#include "utils/color_utils.h"
#include "physics.h"
#include "engine.h"

WindowManager *wm = nullptr;

void World::initialize(int width, int height)
{
    World::width = width;
    World::height = height;
    World::grid.resize(width * height);
    World::flat_cell_color_list.resize(width * height);

    wm = Engine::get().get_window_manager();

    std::cout << "World is initialized with w: " << width << ", height: " << height << ", totaling to " << (width * height) << " cells." << std::endl;
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

    case MaterialType::GAS:
        Physics::update_gas(world, x, y);
        break;

    case MaterialType::STONE:
        break;
    }
}

void World::update()
{
    // Increment frame counter instead of clearing 240k cells
    world_frame++;

    auto flag_start = std::chrono::high_resolution_clock::now();
    auto flag_end = std::chrono::high_resolution_clock::now();
    double flag_time = std::chrono::duration<double, std::milli>(flag_end - flag_start).count();

    InputManager *im = Engine::get().get_input_manager();

    auto brush_start = std::chrono::high_resolution_clock::now();
    if (im->get_is_cursor_down())
    {
        int cursor_x = im->get_cursor_x();
        int cursor_y = im->get_cursor_y() - wm->get_top_panel_rect().height;

        if (cursor_x >= 0 && cursor_x <= width && cursor_y >= 0 && cursor_y <= height)
        {
            int brush_radius = 24;

            for (int y = cursor_y - brush_radius; y <= cursor_y + brush_radius; y++)
            {
                for (int x = cursor_x - brush_radius; x <= cursor_x + brush_radius; x++)
                {
                    if (x < 0 || x >= width || y < 0 || y >= height)
                        continue;

                    double xx = x - cursor_x;
                    double yy = y - cursor_y;
                    double distance = std::sqrt((xx * xx) + (yy * yy));

                    if (distance <= brush_radius && get_cell_id(x, y) == MaterialType::EMPTY)
                        set_cell(x, y, MaterialType::SAND);
                }
            }
        }
    }
    auto brush_end = std::chrono::high_resolution_clock::now();
    double brush_time = std::chrono::duration<double, std::milli>(brush_end - brush_start).count();

    int frame_count = Engine::get().get_ticks_passed();

    auto physics_start = std::chrono::high_resolution_clock::now();
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
    return (y * width) + x;
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
    int idx1 = coords_to_index(x1, y1);
    int idx2 = coords_to_index(x2, y2);

    uint8_t id = grid[idx1].id;
    grid[idx1].id = MaterialType::EMPTY;
    grid[idx2].id = id;

    flat_cell_color_list[idx1] = mat_colors[MaterialType::EMPTY];
    flat_cell_color_list[idx2] = mat_colors[id];
}

const Cell &World::get_cell(int x, int y) const
{
    return grid[coords_to_index(x, y)];
}

const Cell &World::get_cell(int index) const
{
    return grid[index];
}