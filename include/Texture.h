/// \brief Handles textures input / drawing (velocity arrows)
/// \author Josh Bailey
/// \version 1.0
/// \date 23/05/21 Updated to NCCA Coding Standard
/// Revision History:
///
/// \todo

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL2/SDL.h>

#include <string>

class Texture
{
    public:
        Texture();
        ~Texture();

        bool Load(std::string path, SDL_Renderer* _renderer);
        void Free();
        void SetColour(Uint8 _r, Uint8 _g, Uint8 _b);
        void Draw(int _x, int _y, SDL_Rect* _rect, float _angle, int _scale, SDL_Renderer* _renderer);

        int GetWidth();
        int GetHeight();

    private:
        SDL_Texture* m_texture = NULL;

        // Image dimensions
        int m_width = 0;
        int m_height = 0;
};

#endif // _TEXTURE_H_