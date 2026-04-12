#pragma once
#include <vector>
#include <cstdint>

struct Cell
{
    uint8_t id;
    uint32_t color; // ARGB8888
};

class World
{
public:
    World(int width, int height);
    ~World() = default;

    void update();
    const uint32_t *get_pixel_buffer() const;

private:
    int m_width;
    int m_height;
    std::vector<Cell> grid;
    std::vector<uint32_t> pixel_buffer;

    int get_index(int x, int y) const;
};