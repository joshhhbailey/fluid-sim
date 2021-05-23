/// \brief Manages keyboard inputs from the user
/// \author Josh Bailey
/// \version 1.0
/// \date 23/05/21 Updated to NCCA Coding Standard
/// Revision History:
///
/// \todo

#ifndef KEYBOARD_MANAGER_H_
#define KEYBOARD_MANAGER_H_

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

#endif  // _KEYBOARD_MANAGER_H_