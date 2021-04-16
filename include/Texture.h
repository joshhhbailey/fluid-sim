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