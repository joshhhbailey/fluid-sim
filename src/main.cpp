///
/// @file main.cpp
/// @brief Program entry, creats SDL context

#include "SDLScene.h"

#include <iostream>

int main(int argc, char* args[])
{
    SDLScene scene;
    if (scene.Initialise())
    {
        scene.GameLoop();
    }
    else
    {
        std::cout << "SDL failed to initialise!\n";
    }
    return 0;
}