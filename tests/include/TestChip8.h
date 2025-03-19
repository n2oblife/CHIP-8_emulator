#include "Chip8.h"
#include "Tests_common.h"

class TestChip8 : public testing::Test
{
protected:
    Chip8 chip8;
    uint8_t keys[NUM_KEYS] = {0};
    SDL_Event event;
    std::string tempFilePath = "";

    void SetUp() override {
        chip8.InitChip8();
    };

    void TearDown() override {
        if (std::filesystem::exists(tempFilePath))
            std::filesystem::remove(tempFilePath);
        if (tempFilePath != "")
            tempFilePath = "";
    };

    // ====== Friends to get access to private parameters ======

    inline std::array<uint8_t, NUM_REGISTERS>& get_registers(){
        return chip8.registers;
    }
    
    inline std::array<uint8_t, MEMORY_SIZE>& get_memory() {
        return chip8.memory;
    }

    inline uint16_t& get_index() {
        return chip8.index;
    }
    
    inline uint16_t& get_pc() {
        return chip8.pc;
    }

    inline std::array<uint16_t, STACK_SIZE>& get_stack() {
        return chip8.stack;
    }

    inline uint8_t& get_sp() {
        return chip8.sp;
    }

    inline uint8_t& get_delayTimer() {
        return chip8.delayTimer;
    }

    inline uint8_t& get_soundTimer() {
        return chip8.soundTimer;
    }

    inline std::array<uint8_t, NUM_KEYS>& get_keypad() {
        return chip8.keypad;  // Return a reference to the keypad array
    }

    inline std::array<uint32_t, VIDEO_HEIGHT * VIDEO_WIDTH>& get_video() {
        return chip8.video;  // Return a reference to the video array
    }
    
    inline uint16_t& get_opcode() {
        return chip8.opcode;
    }


};
