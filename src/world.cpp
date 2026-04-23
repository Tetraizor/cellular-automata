#include "world.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <chrono>

static uint32_t vary_color(uint32_t base, uint8_t variance, uint32_t &rng)
{
    if (variance == 0) return base;

    rng ^= rng << 13;
    rng ^= rng >> 17;
    rng ^= rng << 5;

    int v = variance;
    int rn = (int)((rng & 0xFF)        % (2 * v + 1)) - v;
    int gn = (int)(((rng >> 8) & 0xFF) % (2 * v + 1)) - v;
    int bn = (int)(((rng >>16) & 0xFF) % (2 * v + 1)) - v;

    int r = std::max(0, std::min(255, (int)((base >> 16) & 0xFF) + rn));
    int g = std::max(0, std::min(255, (int)((base >>  8) & 0xFF) + gn));
    int b = std::max(0, std::min(255, (int)( base        & 0xFF) + bn));

    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

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
    bool placing = im->get_is_cursor_down();
    bool erasing = im->get_is_right_cursor_down();

    if (placing || erasing)
    {
        int cursor_x = im->get_cursor_x();
        int cursor_y = im->get_cursor_y() - wm->get_top_panel_rect().height;

        if (cursor_x >= 0 && cursor_x <= width && cursor_y >= 0 && cursor_y <= height)
        {
            int brush_radius = Engine::get().get_brush_radius();

            for (int y = cursor_y - brush_radius; y <= cursor_y + brush_radius; y++)
            {
                for (int x = cursor_x - brush_radius; x <= cursor_x + brush_radius; x++)
                {
                    if (x < 0 || x >= width || y < 0 || y >= height)
                        continue;

                    double xx = x - cursor_x;
                    double yy = y - cursor_y;
                    double distance = std::sqrt((xx * xx) + (yy * yy));

                    if (distance > brush_radius)
                        continue;

                    if (placing && get_cell_id(x, y) == MaterialType::EMPTY)
                        set_cell(x, y, Engine::get().get_selected_material());
                    else if (erasing && get_cell_id(x, y) != MaterialType::EMPTY)
                        set_cell(x, y, MaterialType::EMPTY);
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

    // Animate water: write a slowly-scrolling brightness wave so settled
    // water shimmers rather than looking like static powder.
    {
        uint32_t base = mat_colors[MaterialType::WATER];
        int base_r = (base >> 16) & 0xFF;
        int base_g = (base >>  8) & 0xFF;
        int base_b =  base        & 0xFF;
        int t = (int)(frame_count >> 2); // changes every 4 ticks (~36 hz)

        for (int i = 0; i < width * height; ++i)
        {
            if (grid[i].id != MaterialType::WATER)
                continue;

            int x = i % width;
            int y = i / width;

            // Diagonal spatial phase + slow time → travelling wave
            int phase = (x * 3 + y * 7) & 0x3F;
            int wave  = (phase + t) & 0x3F;
            int brightness = (wave < 32 ? wave : 64 - wave) / 4 - 4; // -4..+4

            int r = std::max(0, std::min(255, base_r + brightness));
            int g = std::max(0, std::min(255, base_g + brightness));
            int b = std::max(0, std::min(255, base_b + brightness));
            flat_cell_color_list[i] = (0xFF << 24) | (r << 16) | (g << 8) | b;
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
    grid[index].id = new_id;
    flat_cell_color_list[index] = vary_color(mat_colors[new_id], mat_variance[new_id], rng_state);
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

    grid[idx2].id            = grid[idx1].id;
    flat_cell_color_list[idx2] = flat_cell_color_list[idx1]; // preserve varied color

    grid[idx1].id            = MaterialType::EMPTY;
    flat_cell_color_list[idx1] = mat_colors[MaterialType::EMPTY];
}

void World::swap_cells(int x1, int y1, int x2, int y2)
{
    int idx1 = coords_to_index(x1, y1);
    int idx2 = coords_to_index(x2, y2);

    std::swap(grid[idx1].id,              grid[idx2].id);
    std::swap(flat_cell_color_list[idx1], flat_cell_color_list[idx2]);
}

const Cell &World::get_cell(int x, int y) const
{
    return grid[coords_to_index(x, y)];
}

const Cell &World::get_cell(int index) const
{
    return grid[index];
}