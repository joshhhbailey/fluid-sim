#include <SDL2/SDL.h>

class KeyboardManager
{
    public:
        KeyboardManager();
        ~KeyboardManager();

        bool GetKeyDown(SDL_Scancode _key);
        bool GetKey(SDL_Scancode _key);
        bool GetKeyUp(SDL_Scancode _key);

        void Update();

    private:
        const Uint8* keyState;
        int pressedKeys;
        Uint8* currentPressed;
        Uint8* previousPressed;
};