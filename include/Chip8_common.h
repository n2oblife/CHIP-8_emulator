#pragma once

#include <cstdint>
#include <string>
#include <array>

// Default values for Main's arguments
const int DEFAULT_VIDEO_SCALE = 10;
const int DEFAULT_CYCLE_DELAY = 1;

// Define memory layout constants
const unsigned int START_ADDRESS = 0x200;           // Start address for programs
const unsigned int FONTSET_START_ADDRESS = 0x50;    // Start of the CHIP-8 fontset
const unsigned int FONT_SIZE = 5;    				// Size of the fontset characters
const unsigned int MEMORY_SIZE = 4096;              // 4KB of memory

// Define display resolution
const unsigned int VIDEO_WIDTH = 64;                // 64x32 pixel monochrome display
const unsigned int VIDEO_HEIGHT = 32;               // 64x32 pixel monochrome display
const unsigned int FONTSET_SIZE = 80;               // Size of the CHIP-8 fontset
constexpr std::array<uint8_t, FONTSET_SIZE> fontset = { // CHIP-8 fontset for all inputs 0-F
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

// Define the number of registers and stack size
const unsigned int NUM_REGISTERS = 16;              // 16 8-bit registers (V0-VF)
const unsigned int STACK_SIZE = 16;                 // 16 levels of stack for subroutine calls
const unsigned int NUM_KEYS = 16;                   // 16-key hexadecimal keypad

