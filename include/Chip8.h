#pragma once

#include "Chip8_common.h"
#include <cstdint>

class Chip8
{
public:
    // Constructor to initialize the emulator
    Chip8();

    // Destructor (if necessary)
    ~Chip8() = default;




private:
    // CHIP-8 registers, memory, and other components
    uint8_t     registers[NUM_REGISTERS];           // General-purpose registers (V0-VF)
    uint8_t     memory[MEMORY_SIZE];                // 4KB memory (0x100-0xFFF for programs)
    uint16_t    index;                              // Index register (I)
    uint16_t    pc;                                 // Program counter (PC)
    uint16_t    stack[STACK_SIZE];                  // Stack for subroutine calls
    uint8_t     sp;                                 // Stack pointer
    uint8_t     delayTimer;                         // Delay timer
    uint8_t     soundTimer;                         // Sound timer
    uint8_t     keypad[NUM_KEYS];                   // Input keypad state (16 keys, 0-F)
    uint32_t    video[VIDEO_WIDTH * VIDEO_HEIGHT];  // 64x32 pixel monochrome display
    uint16_t    opcode;                             // Current opcode

    // Internal flag to track when the screen needs to be updated
    bool        draw_flag;

    // Helper methods for opcode processing
    void executeOpcode(uint16_t opcode);  // Execute the current opcode
};