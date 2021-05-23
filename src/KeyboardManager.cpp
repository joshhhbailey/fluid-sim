///
/// @file KeyboardManager.cpp
/// @brief Manages keyboard inputs from the user

#include "KeyboardManager.h"

#include <iostream>

KeyboardManager::KeyboardManager()
{
	keyState = SDL_GetKeyboardState(&pressedKeys);
	currentPressed = new Uint8[pressedKeys];    // Store keys pressed this frame
	previousPressed = new Uint8[pressedKeys];   // ... and last frame
}

KeyboardManager::~KeyboardManager()
{
	delete[] currentPressed;
	delete[] previousPressed;
}

void KeyboardManager::Update()
{
	// Update arrays
	std::memcpy(previousPressed, currentPressed, pressedKeys);
	std::memcpy(currentPressed, keyState, pressedKeys);
}

bool KeyboardManager::GetKeyDown(SDL_Scancode _key)
{
	// Pressing this frame and not last
	if (currentPressed[_key] && !previousPressed[_key])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KeyboardManager::GetKey(SDL_Scancode _key)
{
	// Pressing this frame and last
	if (currentPressed[_key] && previousPressed[_key])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KeyboardManager::GetKeyUp(SDL_Scancode _key)
{
	// Pressing last frame and not now
	if (!currentPressed[_key] && previousPressed[_key])
	{
		return true;
	}
	else
	{
		return false;
	}
}