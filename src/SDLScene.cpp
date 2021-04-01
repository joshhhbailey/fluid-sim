#include "SDLScene.h"
#include "Fluid.h"

#include <iostream>

SDLScene::SDLScene()
{
}

bool SDLScene::Initialise()
{
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError();
    }
    else
    {
        // Create window
        window = SDL_CreateWindow("Grid Based Fluid Sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            std::cout << "Window could not be created! SDL_Error: %s\n" << SDL_GetError();
            success = false;
        }
        else
        {
            // Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError();
				success = false;
			}
            else
            {
                // Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    return success;
}

void SDLScene::GameLoop()
{
    bool quit = false;

    // Initialise mouse position
    SDL_GetMouseState(&mouseX, &mouseY);

    // Event handler
	SDL_Event e;

    // Create fluid
    Fluid fluid(fluidCellSize, SCREEN_SIZE / fluidCellSize, 0.1f, 0, 0);

	// While application is running
	while (!quit)
	{
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    LMBdown = true;
                }
                if (e.button.button == SDL_BUTTON_MIDDLE)
                {
                    MMBdown = true;
                }
                if (e.button.button == SDL_BUTTON_RIGHT)
                {
                    RMBdown = true;
                }
            }
            if (e.type == SDL_MOUSEBUTTONUP)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    LMBdown = false;
                }
                if (e.button.button == SDL_BUTTON_MIDDLE)
                {
                    MMBdown = false;
                }
                if (e.button.button == SDL_BUTTON_RIGHT)
                {
                    RMBdown = false;
                }
            }
		}

        // Keyboard input
        keyboard.Update();
        if (keyboard.GetKeyDown(SDL_SCANCODE_D))
        {
            if (!showGrid)
            {
                showGrid = true;
            }
            else
            {
                showGrid = false;
            }
        }
        if (keyboard.GetKeyDown(SDL_SCANCODE_R))
        {
            fluid.Reset();
        }
        if (keyboard.GetKeyDown(SDL_SCANCODE_ESCAPE))
        {
            quit = true;
        }

        // Mouse button input
        if (MMBdown)
        {
            UpdateMousePosition();
            fluid.AddDensity(mouseX / fluidCellSize, mouseY / fluidCellSize, 100);
            CalculateVelocity();
            fluid.AddVelocity(mouseX / fluidCellSize, mouseY / fluidCellSize, xVel, yVel);
        }
        else if (LMBdown)
        {
            UpdateMousePosition();
            fluid.AddDensity(mouseX / fluidCellSize, mouseY / fluidCellSize, 100);
        }
        else if (RMBdown)
        {
            UpdateMousePosition();
            CalculateVelocity();
            fluid.AddVelocity(mouseX / fluidCellSize, mouseY / fluidCellSize, xVel, yVel);
        }

        // Clear screen
		SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);
		SDL_RenderClear(renderer);

        if (showGrid)
        {
            fluid.ShowGrid(renderer);
        }

        fluid.Update();
        fluid.Draw(renderer);
        fluid.Fade(0.1f);

        // Update screen
		SDL_RenderPresent(renderer);
	}
    Close();
}

void SDLScene::Close()
{
    // Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    // Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

void SDLScene::UpdateMousePosition()
{
    prevMouseX = mouseX;
    prevMouseY = mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
}

void SDLScene::CalculateVelocity()
{
    xVel = float(mouseX - prevMouseX);
    yVel = float(mouseY - prevMouseY);
}