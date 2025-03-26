/**
 * @file Chip8_common.h
 * @brief Constant values, useful for building the emulator
 *
 * This file contains the declaration of the Chip8 global values, to build the CHIP-8 system.
 */

#pragma once

#include <cstdint>
#include <string>
#include <array>

/** @brief Default video scaling factor for rendering. */
const int DEFAULT_VIDEO_SCALE = 10;

/** @brief Default cycle delay for the emulator (in milliseconds). */
const int DEFAULT_CYCLE_DELAY = 1;

/** @brief Start address in memory where CHIP-8 programs are loaded. */
const unsigned int START_ADDRESS = 0x200;

/** @brief Memory address where the CHIP-8 fontset starts. */
const unsigned int FONTSET_START_ADDRESS = 0x50;

/** @brief Size (in bytes) of each character in the fontset. */
const unsigned int FONT_SIZE = 5;

/** @brief Total size of CHIP-8 memory (4KB). */
const unsigned int MEMORY_SIZE = 4096;

/** @brief Width of the CHIP-8 display (in pixels). */
const unsigned int VIDEO_WIDTH = 64;

/** @brief Height of the CHIP-8 display (in pixels). */
const unsigned int VIDEO_HEIGHT = 32;

/** @brief Total size of the CHIP-8 fontset (in bytes). */
const unsigned int FONTSET_SIZE = 80;

/** 
 * @brief CHIP-8 built-in fontset.
 * @details This array stores the 16 hexadecimal characters (0-F) used for rendering.
 */
constexpr std::array<uint8_t, FONTSET_SIZE> fontset = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

/** @brief Number of general-purpose registers (V0-VF). */
const unsigned int NUM_REGISTERS = 16;

/** @brief Stack size for subroutine calls (16 levels). */
const unsigned int STACK_SIZE = 16;

/** @brief Number of keys in the CHIP-8 hexadecimal keypad (0-F). */
const unsigned int NUM_KEYS = 16;
