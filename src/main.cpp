#include <iostream>
#include "engine.h"

// TODO: Separate SDL logic & game loop from main method
int main(int argc, char *argv[])
{
    Engine &engine = Engine::get();

    if (argc <= 1)
    {
        return engine.run();
    }
    else if (argc == 3)
    {
        int world_width, world_height;
        try
        {
            world_width = std::stoi(argv[1]);
            world_height = std::stoi(argv[2]);
        }
        catch (std::invalid_argument e)
        {
            std::cout << "Invalid argument type" << std::endl;
            return -1;
        }

        return engine.run(world_width, world_height);
    }
    else if (argc == 4)
    {
        int world_width, world_height, zoom_factor;

        try
        {
            world_width = std::stoi(argv[1]);
            world_height = std::stoi(argv[2]);
            zoom_factor = std::stoi(argv[3]);
        }
        catch (std::invalid_argument e)
        {
            std::cout << "Invalid argument type" << std::endl;
            return -1;
        }

        return engine.run(world_width, world_height, zoom_factor);
    }
    else
    {
        std::cout << "Invalid arguments" << std::endl;
    }
}