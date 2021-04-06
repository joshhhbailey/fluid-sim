#include <SDL2/SDL.h>

#include "KeyboardManager.h"

class SDLScene
{
    public:
        SDLScene();

        bool Initialise();
        void GameLoop();
        void Close();

        void UpdateMousePosition();
        void CalculateVelocity();

    private:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        KeyboardManager keyboard;

        const int SCREEN_SIZE = 512;
        const int fluidCellSize = 32;

        // Mouse position
        int prevMouseX;
        int prevMouseY;
        int mouseX;
        int mouseY;
        float xVel;
        float yVel;

        // Mouse buttons
        bool LMBdown = false;
        bool MMBdown = false;
        bool RMBdown = false;

        // Debug
        bool showGrid = false;
        bool showVelocity = false;
};