///
/// @file Texture.cpp
/// @brief Handles textures input / drawing (velocity arrows)

#include <SDL_image.h>

#include "Texture.h"

#include <iostream>

Texture::Texture()
{
}

Texture::~Texture()
{
	Free();
}

bool Texture::Load(std::string path, SDL_Renderer* _renderer)
{
	// Clear exisiting texture
	Free();

	SDL_Texture* texture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load image! SDL_image Error: " << path.c_str() << "\n" << IMG_GetError();
	}
	else
	{
		// Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
		if (texture == NULL)
		{
			std::cout << "Unable to create texture from: " << path.c_str() << "\nSDL Error: " << SDL_GetError();
		}
		else
		{
			// Get image dimensions
			m_width = loadedSurface->w;
			m_height = loadedSurface->h;
		}

		// Free old surface
		SDL_FreeSurface(loadedSurface);
	}
	m_texture = texture;
	return m_texture != NULL;
}

void Texture::Free()
{
	// Free texture, if it exists
	if (m_texture != NULL)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
		m_width = 0;
		m_height = 0;
	}
}

void Texture::SetColour(Uint8 _r, Uint8 _g, Uint8 _b)
{
	SDL_SetTextureColorMod(m_texture, _r, _g, _b);
}

void Texture::Draw(int _x, int _y, SDL_Rect* _rect, float _angle, int _scale, SDL_Renderer* _renderer)
{
	// Scale texture to grid cell dimensions
	SDL_Rect renderQuad = {_x, _y, m_width / (int)pow(2, 5 - _scale), m_height / (int)pow(2, 5 - _scale) };

	if (_rect != NULL)
	{
		renderQuad.w = _rect->w;
		renderQuad.h = _rect->h;
	}

	SDL_RenderCopyEx(_renderer, m_texture, _rect, &renderQuad, _angle, NULL, SDL_FLIP_NONE);
}

int Texture::GetWidth()
{
	return m_width;
}

int Texture::GetHeight()
{
	return m_height;
}