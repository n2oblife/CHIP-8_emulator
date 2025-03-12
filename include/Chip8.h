#pragma once

#include <cstdint>
#include <random>
#include <chrono>
#include <cstring>
#include <fstream>
#include "Chip8_common.h"

class Chip8
{
public:
    Chip8();
    ~Chip8() = default;

    // ====== Getters of the class ======

    inline uint32_t* Chip8::get_video(){
        return this->video;
    }
    
    inline uint8_t* Chip8::get_keypad(){
        return this->keypad;
    }

    // ====== Actual CPU functions ======

    // Load a ROM file into memory
    void LoadROM(char const* filename);

    // Clear display
    void OP_00E0();

    // Return from a subroutine
    void OP_00EE();

    // Jump to address NNN
    void OP_1nnn();

    // Call subroutine at NNN
    void OP_2nnn();

    // Skip next instruction if Vx == kk
    void OP_3xkk();

    // Skip next instruction if Vx != kk
    void OP_4xkk();

    // Skip next instruction if Vx == Vy
    void OP_5xy0();

    // Set Vx = kk
    void OP_6xkk();

    // Set Vx = Vx + kk
    void OP_7xkk();

    // Set Vx = Vy
    void OP_8xy0();

    // Set Vx = Vx | Vy
    void OP_8xy1();

    // Set Vx = Vx & Vy
    void OP_8xy2();

    // Set Vx = Vx ^ Vy (XOR)
    void OP_8xy3();

    // Set Vx = Vx + Vy, set VF = carry (sum which handles overflow)
    void OP_8xy4();

    /*
    Set Vx = Vx - Vy, set VF = NOT borrow (Vx > Vy), 
    (underflow is just flagged)
    */ 
    void OP_8xy5();

    /* 
    Set Vx = Vx >> 1, set VF = carry (LSB before shift), 
    tells if Vx is divisible by 2
    */ 
    void OP_8xy6();

    /*
    Set Vx = Vy - Vx, set VF = NOT borrow (Vy > Vx), 
    (underflow is just flagged)
    */ 
    void OP_8xy7();

    /*
    Set Vx = Vx << 1 = 2*Vx, set VF = carry (MSB before shift), 
    Vf tells if there was overflow
    */ 
    void OP_8xyE();

    // Skip next instruction if Vx != Vy
    void OP_9xy0();

    // Set I = nnn (address)
    void OP_Annn();

    // Jump to location nnn + V0
    void OP_Bnnn();

    // Set Vx = random byte & kk
    void OP_Cxkk();

    /*
    Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    The interpreter reads n bytes from memory, starting at the address stored in I.
    These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
    Sprites are XORed onto the existing screen. If this causes any pixels to be erased,
    VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is
    outside the coordinates of the display, it wraps around to the opposite side of the screen.
    index of the sprite is stored in I and already set by the caller
    */
    void OP_Dxyn();

    // Skip next instruction if key with the value of Vx is pressed
    void OP_Ex9E();

    // Skip next instruction if key with the value of Vx is not pressed
    void OP_ExA1();

    // Set Vx = delay timer value
    void OP_Fx07();

    /**
    Wait for a key press, store the value of the key in Vx
    The easiest way to “wait” is to decrement the PC by 2 whenever a keypad value is not detected. 
    This has the effect of running the same instruction repeatedly.
     */
    void OP_Fx0A();

    /*
    Wait for a key press, store the value of the key in Vx (optimized)
    The easiest way to “wait” is to decrement the PC by 2 whenever a keypad value is not detected. 
    This has the effect of running the same instruction repeatedly.
    */
    void OP_Fx0A_optimized();

    // Set delay timer = Vx
    void OP_Fx15();

    // Set sound timer = Vx
    void OP_Fx18();

    // Set I = I + Vx
    void OP_Fx1E();

    // Set I = location of font sprite for digit Vx
    void OP_Fx29();

    /*
    Store Binary-Coded Decimal (BCD = decimal representation with each element stored in a different memory) 
    representation of Vx in memory locations I, I+1, and I+2.
    The interpreter takes the decimal value of Vx, 
    and places the hundreds digit in memory at location in I, 
    the tens digit at location I+1, 
    and the ones digit at location I+2.
    */
    void OP_Fx33();

    // Store registers V0 through Vx in memory starting at location I (previously set by the caller)
    void OP_Fx55();

    // Read registers V0 through Vx from memory starting at location I (previously set by the caller)
    void OP_Fx65();

    //Opcode table functions for opcode OP_00E-
    void Table0();

    //Opcode table functions for opcode OP_8xy-
    void Table8();

    /*
    Opcode table functions for opcode OP_Ex--
    The only difference between the two functions is the last hex digit
    */
    void TableE();

    /*
    Opcode table functions for opcode OP_Fx--
    The only difference between the two functions is the last hex digit
    */
    void TableF();

    // Opcode functions NULL implementation for when an opcode is not implemented
    void OP_NULL(){};

    // Emulate one cycle of the CHIP-8 CPU (fetch, decode, execute)
    void Cycle();


private:
    
    friend class TestChip8;

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

    // Random number generation
    std::default_random_engine randGen;
    std::uniform_int_distribution<unsigned int> randByte; // must be cawsted to uint8_t

    // Opcode table functions definitions with overhead because many unused memory
    typedef void (Chip8::*Chip8Func)();
        Chip8Func table[0xF + 1];
        Chip8Func table0[0xE + 1];
        Chip8Func table8[0xE + 1];
        Chip8Func tableE[0xE + 1];
        Chip8Func tableF[0x65 + 1];

    // ====== Some private functions for the inside use ======

    std::vector<uint8_t> filenameHandling(char const* filename);

};