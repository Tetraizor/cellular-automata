#include <iostream>
#include "engine.h"

// TODO: Separate SDL logic & game loop from main method
int main(int arg, char *argv[])
{
    Engine &engine = Engine::get();
    return engine.run();
}