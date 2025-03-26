/**
 * @file Chip8.h
 * @brief CHIP-8 Emulator Class
 *
 * This file contains the declaration of the Chip8 class, which emulates a CHIP-8 system.
 * The class provides functionality to load ROMs, execute instructions, and manage memory, registers, and timers.
 */

 #pragma once

 #include <cstdint>
 #include <random>
 #include <chrono>
 #include <cstring>
 #include <fstream>
 #include <filesystem>
 
 #include "Chip8_common.h"
 
 /**
  * @class Chip8
  * @brief CHIP-8 Emulator
  *
  * This class emulates a CHIP-8 CPU, providing methods to load ROMs, execute instructions,
  * and interact with the display and keypad.
  */
 class Chip8
 {
 public:
     /**
      * @brief Constructor: Initializes the CHIP-8 emulator.
      */
     Chip8();
 
     /**
      * @brief Default destructor.
      */
     ~Chip8() = default;
 
     // ====== Getters of the class ======
 
     /**
      * @brief Get the current state of the keypad.
      * @return An array representing the keypad state (pressed keys).
      */
     inline std::array<uint8_t, NUM_KEYS> get_keypad() const { return this->keypad; }
 
     /**
      * @brief Get the current state of the video display.
      * @return An array representing the monochrome 64x32 display.
      */
     inline std::array<uint32_t, VIDEO_HEIGHT * VIDEO_WIDTH> get_video() const { return this->video; }
 
     // ====== CPU Functions ======
 
     /**
      * @brief Loads a ROM file into memory.
      * @param filename Path to the ROM file.
      */
     void LoadROM(const std::string& filename);
 
     /** @brief Clears the display. */
     void OP_00E0();
 
     /** @brief Returns from a subroutine. */
     void OP_00EE();
 
     /** @brief Jumps to address NNN. */
     void OP_1nnn();
 
     /** @brief Calls subroutine at address NNN. */
     void OP_2nnn();
 
     /** @brief Skips next instruction if Vx == kk. */
     void OP_3xkk();
 
     /** @brief Skips next instruction if Vx != kk. */
     void OP_4xkk();
 
     /** @brief Skips next instruction if Vx == Vy. */
     void OP_5xy0();
 
     /** @brief Sets Vx = kk. */
     void OP_6xkk();
 
     /** @brief Sets Vx = Vx + kk. */
     void OP_7xkk();
 
     /** @brief Sets Vx = Vy. */
     void OP_8xy0();
 
     /** @brief Performs bitwise OR: Vx = Vx | Vy. */
     void OP_8xy1();
 
     /** @brief Performs bitwise AND: Vx = Vx & Vy. */
     void OP_8xy2();
 
     /** @brief Performs bitwise XOR: Vx = Vx ^ Vy. */
     void OP_8xy3();
 
     /** @brief Adds Vx and Vy, storing result in Vx and setting VF if overflow occurs. */
     void OP_8xy4();
 
     /** @brief Subtracts Vy from Vx, storing result in Vx and setting VF to NOT borrow. */
     void OP_8xy5();
 
     /** @brief Shifts Vx right by 1, storing the least significant bit in VF. */
     void OP_8xy6();
 
     /** @brief Sets Vx = Vy - Vx, setting VF to NOT borrow. */
     void OP_8xy7();
 
     /** @brief Shifts Vx left by 1, storing the most significant bit in VF. */
     void OP_8xyE();
 
     /** @brief Skips next instruction if Vx != Vy. */
     void OP_9xy0();
 
     /** @brief Sets I = NNN. */
     void OP_Annn();
 
     /** @brief Jumps to location NNN + V0. */
     void OP_Bnnn();
 
     /** @brief Sets Vx = random byte & kk. */
     void OP_Cxkk();
 
     /**
      * @brief Draws a sprite at coordinate (Vx, Vy).
      * @details The sprite is `n` bytes in height and starts at memory location I.
      * VF is set to 1 if any pixels are erased due to collision.
      */
     void OP_Dxyn();
 
     /** @brief Skips next instruction if key Vx is pressed. */
     void OP_Ex9E();
 
     /** @brief Skips next instruction if key Vx is not pressed. */
     void OP_ExA1();
 
     /** @brief Sets Vx to the value of the delay timer. */
     void OP_Fx07();
 
     /**
      * @brief Waits for a key press and stores the value in Vx.
      * @details Execution pauses until a key is pressed.
      */
     void OP_Fx0A();
 
     /**
      * @brief Optimized version of OP_Fx0A.
      */
     void OP_Fx0A_optimized();
 
     /** @brief Sets the delay timer to Vx. */
     void OP_Fx15();
 
     /** @brief Sets the sound timer to Vx. */
     void OP_Fx18();
 
     /** @brief Sets I = I + Vx. */
     void OP_Fx1E();
 
     /** @brief Sets I to the location of the sprite for digit Vx. */
     void OP_Fx29();
 
     /**
      * @brief Stores the BCD representation of Vx at memory locations I, I+1, and I+2.
      */
     void OP_Fx33();
 
     /** @brief Stores registers V0 through Vx in memory starting at I. */
     void OP_Fx55();
 
     /** @brief Reads registers V0 through Vx from memory starting at I. */
     void OP_Fx65();
 
     /** @brief Handles OP_00E* opcode. */
     void Table0();
 
     /** @brief Handles OP_8xy* opcode. */
     void Table8();
 
     /** @brief Handles OP_Ex** opcode. */
     void TableE();
 
     /** @brief Handles OP_Fx** opcode. */
     void TableF();
 
     /** @brief Handles unimplemented opcodes (NOP). */
     void OP_NULL(){};
 
     /** @brief Executes one cycle of the CHIP-8 CPU. */
     void Cycle();
 
 private:
     // CHIP-8 CPU registers, memory, and peripherals
     std::array<uint8_t, NUM_REGISTERS> registers; /**< General-purpose registers (V0-VF). */
     std::array<uint8_t, MEMORY_SIZE> memory; /**< Memory (4KB). */
     uint16_t index; /**< Index register (I). */
     uint16_t pc; /**< Program counter (PC). */
     std::array<uint16_t, STACK_SIZE> stack; /**< Stack for subroutine calls. */
     uint8_t sp; /**< Stack pointer. */
     uint8_t delayTimer; /**< Delay timer. */
     uint8_t soundTimer; /**< Sound timer. */
     std::array<uint8_t, NUM_KEYS> keypad; /**< Keypad state (16 keys). */
     std::array<uint32_t, VIDEO_HEIGHT * VIDEO_WIDTH> video; /**< 64x32 monochrome display. */
     uint16_t opcode; /**< Current opcode being executed. */
     
     bool draw_flag; /**< Flag indicating if the display needs updating. */
 
     /** @brief Random number generator engine. */
     std::default_random_engine randGen;

     /** 
      * @brief Uniform distribution for random number generation.
      * @details Used for generating random bytes, typically in the OP_Cxkk instruction.
      */
     std::uniform_int_distribution<unsigned int> randByte;

     /** @brief Typedef for CHIP-8 opcode function pointers. */
     using Chip8Func = void (Chip8::*)();

     /** 
      * @brief Main opcode function table (0x0 - 0xF).
      * @details Maps opcode prefixes to their corresponding handler functions.
      */
     std::array<Chip8Func, 0x10> table{};

     /** 
      * @brief Opcode table for 0x0 series instructions.
      * @details Handles opcodes OP_00E0 and OP_00EE.
      */
     std::array<Chip8Func, 0xF + 1> table0{};

     /** 
      * @brief Opcode table for 0x8 series instructions.
      * @details Handles arithmetic and logic operations involving Vx and Vy.
      */
     std::array<Chip8Func, 0xF + 1> table8{};

     /** 
      * @brief Opcode table for 0xE series instructions.
      * @details Handles keypress-related operations.
      */
     std::array<Chip8Func, 0xF + 1> tableE{};

     /** 
      * @brief Opcode table for 0xF series instructions.
      * @details Handles timer operations, memory management, and input handling.
      */
     std::array<Chip8Func, 0x66> tableF{};

     /** 
      * @brief Checks and processes the ROM file before loading it.
      * @param filename Path to the ROM file.
      * @return A vector containing the ROM's binary data.
      */
     std::vector<uint8_t> filenameHandling(const std::string& filename);

     /** @brief Initializes CHIP-8 system components (memory, registers, etc.). */
     void InitChip8();

     /** @brief Initializes opcode function tables. */
     void InitializeTables();

     /** @brief Allows TestChip8 to access private members for unit testing. */
     friend class TestChip8;

 };
 