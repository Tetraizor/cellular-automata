#include "engine.h"

#include <iostream>
#include <chrono>

#include "world.h"

Engine::Engine() : ticks(0), millis(0), is_running(true) {}
Engine::~Engine() {}

int Engine::run()
{
    std::cout << "Starting the engine..." << std::endl;

    World world(800, 600);

    if (!wm.initialize(800, 600))
        return 1;

    if (!renderer.initialize(800, 600, wm.get_sdl_renderer_ptr()))
        return 1;

    renderer.set_target_world(&world);

    // Initialize game loop

    const double MAX_FRAME_TIME = 250.0; // For protection against sprial of death
    double accumulator = 0;
    uint64_t loop_iteration_count = 0;

    auto previous_time = std::chrono::steady_clock::now();

    while (is_running)
    {
        // Main loop
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> time_span = current_time - previous_time;
        double elapsed_ms = time_span.count();
        previous_time = current_time;

        if (elapsed_ms > MAX_FRAME_TIME)
        {
            std::cout << "Warning: Lag spike! Dropping "
                      << (elapsed_ms - MAX_FRAME_TIME) << "ms of time." << std::endl;

            elapsed_ms = MAX_FRAME_TIME;
        }

        millis += elapsed_ms;
        accumulator += elapsed_ms;

        // TODO: Handle input

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                is_running = false;
            }
        }

        while (accumulator >= 1000.0 / FIXED_TICK_RATE)
        {
            // TODO: Game logic

            // TODO: Physics logic
            world.update();

            // TODO: Rendering
            renderer.render();

            ticks++;
            accumulator -= 1000.0 / FIXED_TICK_RATE;
        }

        SDL_Delay(1); // To prevent busy waiting to some degree, temporary fix until seperate logic/render threads

        loop_iteration_count++;
    }

    std::cout << "Stopping the engine..." << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Total tick count: " << ticks << "\tTotal busy wait count: " << loop_iteration_count << std::endl;

    return 0;
}