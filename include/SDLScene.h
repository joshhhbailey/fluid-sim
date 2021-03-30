#include <SDL2/SDL.h>

class SDLScene
{
    public:
        SDLScene();
        bool Initialise();
        void GameLoop();
        void Close();

    private:
        // The window we'll be rendering to
        SDL_Window* window = NULL;

        //The window renderer
        SDL_Renderer* renderer = NULL;

        // Screen dimension constants
        const int SCREEN_SIZE = 256;

        // Mouse position
        int prevMouseX;
        int prevMouseY;
        int mouseX;
        int mouseY;
        bool LMBdown = false;
};