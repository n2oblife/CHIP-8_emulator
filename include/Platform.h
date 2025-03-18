#pragma once

#include "Chip8_common.h"
#include <cstdint>
#include <SDL2/SDL.h>

class Platform
{
    public:
        Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);

        ~Platform();

        void Update(const void* buffer, int pitch);

        bool ProcessInput(uint8_t* keys);

    private:
        friend class TestPlatform;
        SDL_Window* window{};
        SDL_Renderer* renderer{};
        SDL_Texture* texture{};
};