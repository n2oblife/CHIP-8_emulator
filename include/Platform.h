/**
 * @file Platform.h
 * @brief Platform and input output handling Class
 *
 * This file contains the declaration of the Platform class, which emulates the keypad and the screen.
 * The class provides functionality to update the screen and the keys input.
 */

#pragma once

#include "Chip8_common.h"
#include <cstdint>
#include <SDL2/SDL.h>

/**
 * @class Platform
 * @brief Handles the window, rendering, and input for the CHIP-8 emulator using SDL.
 */
class Platform
{
public:
    /**
     * @brief Constructs a Platform object.
     * 
     * @param title The title of the window.
     * @param windowWidth The width of the window in pixels.
     * @param windowHeight The height of the window in pixels.
     * @param textureWidth The width of the CHIP-8 screen texture.
     * @param textureHeight The height of the CHIP-8 screen texture.
     */
    Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);

    /**
     * @brief Destroys the Platform object and cleans up SDL resources.
     */
    ~Platform();

    /**
     * @brief Updates the display with new pixel data.
     * 
     * @param buffer A pointer to the pixel data buffer.
     * @param pitch The number of bytes per row in the buffer.
     */
    void Update(const void* buffer, int pitch);

    /**
     * @brief Processes user input and updates the CHIP-8 keypad state.
     * 
     * @param keys Pointer to the CHIP-8 keypad state array (16 keys).
     * @return True if the user wants to quit, otherwise false.
     */
    bool ProcessInput(uint8_t* keys);

private:
    friend class TestPlatform; /**< Allows unit tests to access private members. */

    SDL_Window* window{};   /**< Pointer to the SDL window. */
    SDL_Renderer* renderer{}; /**< Pointer to the SDL renderer. */
    SDL_Texture* texture{}; /**< Pointer to the SDL texture used for rendering. */
};
