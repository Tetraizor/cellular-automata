#pragma once

#include <cstdlib>

#include "world.h"

namespace Physics
{
    inline bool is_in_bounds(const World &world, int x, int y)
    {
        return x >= 0 && x < world.get_width() && y >= 0 && y < world.get_height();
    }

    inline bool is_cell_empty(const World &world, int x, int y)
    {
        if (!is_in_bounds(world, x, y))
            return false;
        return world.get_cell_id(x, y) == MaterialType::EMPTY;
    }

    inline uint32_t fast_rand(uint32_t &state)
    {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    inline void mark_updated(World &world, int x, int y)
    {
        if (is_in_bounds(world, x, y))
        {
            int index = world.coords_to_index(x, y);
            const_cast<Cell &>(world.get_cell(index)).update_frame = world.get_world_frame();
        }
    }

    inline bool is_updated(const World &world, int x, int y)
    {
        if (!is_in_bounds(world, x, y))
            return false;
        return world.get_cell(x, y).update_frame == world.get_world_frame();
    }

    inline void update_powder(World &world, int x, int y)
    {
        if (is_updated(world, x, y))
            return;

        if (is_cell_empty(world, x, y + 1))
        {
            world.move_cell(x, y, x, y + 1);
            mark_updated(world, x, y + 1);
            return;
        }

        if (is_in_bounds(world, x, y + 1) && world.get_cell_id(x, y + 1) == MaterialType::WATER)
        {
            world.swap_cells(x, y, x, y + 1);
            mark_updated(world, x, y + 1);
            return;
        }

        if (is_in_bounds(world, x, y + 1) && world.get_cell_id(x, y + 1) == MaterialType::GAS)
        {
            world.swap_cells(x, y, x, y + 1);
            mark_updated(world, x, y + 1);
            return;
        }

        int direction = (fast_rand(world.rng_state) & 1) ? 1 : -1;

        if (is_cell_empty(world, x + direction, y + 1))
        {
            world.move_cell(x, y, x + direction, y + 1);
            mark_updated(world, x + direction, y + 1);
            return;
        }

        if (is_in_bounds(world, x + direction, y + 1) && world.get_cell_id(x + direction, y + 1) == MaterialType::GAS)
        {
            world.swap_cells(x, y, x + direction, y + 1);
            mark_updated(world, x + direction, y + 1);
            return;
        }

        if (is_cell_empty(world, x - direction, y + 1))
        {
            world.move_cell(x, y, x - direction, y + 1);
            mark_updated(world, x - direction, y + 1);
            return;
        }

        if (is_in_bounds(world, x - direction, y + 1) && world.get_cell_id(x - direction, y + 1) == MaterialType::GAS)
        {
            world.swap_cells(x, y, x - direction, y + 1);
            mark_updated(world, x - direction, y + 1);
            return;
        }
    }

    inline void update_liquid(World &world, int x, int y)
    {
        if (is_updated(world, x, y))
            return;

        if (is_cell_empty(world, x, y + 1))
        {
            world.move_cell(x, y, x, y + 1);
            mark_updated(world, x, y + 1);
            return;
        }

        if (is_in_bounds(world, x, y + 1) && world.get_cell_id(x, y + 1) == MaterialType::GAS)
        {
            world.swap_cells(x, y, x, y + 1);
            mark_updated(world, x, y + 1);
            return;
        }

        int spread_direction = (fast_rand(world.rng_state) & 1) ? 1 : -1;

        if (is_cell_empty(world, x + spread_direction, y + 1))
        {
            world.move_cell(x, y, x + spread_direction, y + 1);
            mark_updated(world, x + spread_direction, y + 1);
            return;
        }

        if (is_in_bounds(world, x + spread_direction, y + 1) && world.get_cell_id(x + spread_direction, y + 1) == MaterialType::GAS)
        {
            world.swap_cells(x, y, x + spread_direction, y + 1);
            mark_updated(world, x + spread_direction, y + 1);
            return;
        }

        if (is_cell_empty(world, x - spread_direction, y + 1))
        {
            world.move_cell(x, y, x - spread_direction, y + 1);
            mark_updated(world, x - spread_direction, y + 1);
            return;
        }

        if (is_in_bounds(world, x - spread_direction, y + 1) && world.get_cell_id(x - spread_direction, y + 1) == MaterialType::GAS)
        {
            world.swap_cells(x, y, x - spread_direction, y + 1);
            mark_updated(world, x - spread_direction, y + 1);
            return;
        }

        constexpr int MAX_SPREAD = 40;

        for (int i = 1; i <= MAX_SPREAD; ++i)
        {
            int new_x = x + (spread_direction * i);

            if (!is_in_bounds(world, new_x, y))
                break;

            uint8_t id = world.get_cell_id(new_x, y);

            if (id == MaterialType::EMPTY)
            {
                world.move_cell(x, y, new_x, y);
                mark_updated(world, new_x, y);
                return;
            }

            if (id != MaterialType::WATER)
                break;
        }

        for (int i = 1; i <= MAX_SPREAD; ++i)
        {
            int new_x = x - (spread_direction * i);

            if (!is_in_bounds(world, new_x, y))
                break;

            uint8_t id = world.get_cell_id(new_x, y);

            if (id == MaterialType::EMPTY)
            {
                world.move_cell(x, y, new_x, y);
                mark_updated(world, new_x, y);
                return;
            }

            if (id != MaterialType::WATER)
                break;
        }

        world.set_sleeping(x, y, true);
    }

    inline void update_gas(World &world, int x, int y)
    {
        if (is_updated(world, x, y))
            return;

        constexpr int directions_y[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        constexpr int directions_x[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        int start_dir = fast_rand(world.rng_state) % 8;

        for (int i = 0; i < 8; ++i)
        {
            int dir_idx = (start_dir + i) % 8;
            int new_x = x + directions_x[dir_idx];
            int new_y = y + directions_y[dir_idx];

            if (is_cell_empty(world, new_x, new_y))
            {
                world.move_cell(x, y, new_x, new_y);
                mark_updated(world, new_x, new_y);
                return;
            }

            if (is_in_bounds(world, new_x, new_y) && world.get_cell_id(new_x, new_y) == MaterialType::WATER)
            {
                world.swap_cells(x, y, new_x, new_y);
                mark_updated(world, new_x, new_y);
                return;
            }
        }
    }
}