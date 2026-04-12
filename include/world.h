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
    const uint32_t *get_cell_colors() const;

    int get_width() const { return width; }
    int get_height() const { return height; }

private:
    int width;
    int height;
    std::vector<Cell> grid;
    std::vector<uint32_t> flat_cell_color_list;

    int coords_to_index(int x, int y) const;
    std::pair<int, int> index_to_coords(int index) const;

    void set_cell(int x, int y, uint8_t new_id, uint32_t new_color);
};