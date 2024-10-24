#pragma once

// Define memory layout constants
const unsigned int START_ADDRESS = 0x200;    // Start address for programs
const unsigned int FONTSET_START_ADDRESS = 0x50; // Start of the CHIP-8 fontset
const unsigned int MEMORY_SIZE = 4096;       // 4KB of memory

// Define display resolution
const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;

// Define the number of registers and stack size
const unsigned int NUM_REGISTERS = 16;
const unsigned int STACK_SIZE = 16;
const unsigned int NUM_KEYS = 16;

