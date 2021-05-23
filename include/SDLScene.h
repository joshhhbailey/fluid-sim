/// \brief Manages SDL scene and main game loop
/// \author Josh Bailey
/// \version 1.0
/// \date 23/05/21 Updated to NCCA Coding Standard
/// Revision History:
///
/// \todo

#ifndef SDL_SCENE_H_
#define SDL_SCENE_H_

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
        SDL_Window* m_window = NULL;
        SDL_Renderer* m_renderer = NULL;

        KeyboardManager m_keyboard;

        const int m_SCREEN_SIZE = 512;

        // Mouse position
        int m_prevMouseX;
        int m_prevMouseY;
        int m_mouseX;
        int m_mouseY;
        float m_xVel;
        float m_yVel;

        // Mouse buttons
        bool m_LMBdown = false;
        bool m_MMBdown = false;
        bool m_RMBdown = false;

        // Debug
        bool m_showGrid = false;
        bool m_showVelocity = false;
};

#endif // _SDL_SCENE_H_