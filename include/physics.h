#pragma once

#include <cstdlib>

#include "world.h"

namespace Physics
{
    inline uint32_t fast_rand(uint32_t &state)
    {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    inline void update_powder(World &world, int x, int y)
    {
        if (world.get_cell_id(x, y + 1) == MaterialType::EMPTY)
        {
            world.move_cell(x, y, x, y + 1);
            return;
        }

        int direction = (fast_rand(world.rng_state) & 1) ? 1 : -1;

        if (world.get_cell_id(x + direction, y + 1) == MaterialType::EMPTY)
        {
            world.move_cell(x, y, x + direction, y + 1);
            return;
        }

        if (world.get_cell_id(x - direction, y + 1) == MaterialType::EMPTY)
        {
            world.move_cell(x, y, x - direction, y + 1);
            return;
        }
    }

    inline void update_liquid(World &world, int x, int y)
    {
    }

    inline void update_gas(World &world, int x, int y)
    {
    }
}