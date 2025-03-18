#include "Chip8.h"
#include "Tests_common.h"

class TestChip8 : public testing::Test
{
protected:
    Chip8 chip8;
    uint8_t keys[NUM_KEYS] = {0};
    SDL_Event event;
    std::string tempFilePath = "";

    void SetUp() override {};

    void TearDown() override {
        if (std::filesystem::exists(tempFilePath))
            std::filesystem::remove(tempFilePath);
        if (tempFilePath != "")
            tempFilePath = "";
    };

    // ====== Friends to get access to private parameters ======

    inline std::array<uint32_t, VIDEO_HEIGHT * VIDEO_WIDTH>& get_video() {
        return chip8.video;  // Return a reference to the video array
    }
    
    inline std::array<uint8_t, NUM_KEYS>& get_keypad() {
        return chip8.keypad;  // Return a reference to the keypad array
    }

    inline std::array<uint8_t, MEMORY_SIZE>& get_memory() {
        return chip8.memory;
    }

    inline uint16_t& get_opcode() {
        return chip8.opcode;
    }

    inline uint16_t& get_pc() {
        return chip8.pc;
    }

    inline uint8_t& get_delayTimer() {
        return chip8.delayTimer;
    }

    inline uint8_t& get_soundTimer() {
        return chip8.soundTimer;
    }
};

// ====== Testing base class functions ======

// This test checks whether the Platform object can be created and destroyed without crashing.
TEST_F(TestChip8, ConstructorDestructor) {
    Chip8 chip8;
}

TEST_F(TestChip8, GetVideo) {
    auto testData = generateRandomData<uint32_t, VIDEO_WIDTH * VIDEO_HEIGHT>();
    std::copy(testData.begin(), testData.end(), get_video().begin());  // Fill the video array
    ASSERT_EQ(chip8.get_video(), testData) << "The video geter is broken\n";
}

TEST_F(TestChip8, GetKeypad) {
    auto testData = generateRandomData<uint8_t, NUM_KEYS>();
    std::copy(testData.begin(), testData.end(), get_keypad().begin());  // Fill the keypad array
    ASSERT_EQ(chip8.get_keypad(), testData) << "The keypad geter is broken\n";
}

// ====== Testing loading ROM ======

TEST_F(TestChip8, EmptyFilenameError) {
    ASSERT_THROW(chip8.LoadROM(tempFilePath), std::runtime_error) << "Chip8 loaded an empty filename\n";
}

TEST_F(TestChip8, NonExistentFileError) {
    tempFilePath = "just/a/file.test";
    ASSERT_THROW(chip8.LoadROM(tempFilePath), std::runtime_error) << "Chip8 didn't read unexisting file\n";
}

TEST_F(TestChip8, ExtensionError) {
    tempFilePath = "./temp_file.test";
    generateTempFile(tempFilePath);
    ASSERT_THROW(chip8.LoadROM(tempFilePath), std::runtime_error) << "Chip8 didn't handle the file extension\n";
}

TEST_F(TestChip8, BigFileError) {
    tempFilePath = "./temp_file.ch8";
    auto testData = generateTempFile<uint8_t, 8126>(tempFilePath);
    ASSERT_THROW(chip8.LoadROM(tempFilePath), std::runtime_error) << "Chip8 didn't handle the file size\n";
}

TEST_F(TestChip8, LoadingRom) {
    tempFilePath = "./temp_file.ch8";
    auto testData = generateTempFile(tempFilePath);
    ASSERT_NO_THROW(chip8.LoadROM(tempFilePath)) << "Chip8 didn't handle the file extension\n";

    bool correctly_loaded = true;
    auto memory = get_memory();
    for (int i=0; i<testData.size(); i++){
        if (memory[START_ADDRESS+i] != testData[i]) {
            correctly_loaded = false;
            break;
        }
    }
    ASSERT_TRUE(correctly_loaded) << "Data loaded in memory is not equal to the ROM.\n";
}

TEST_F(TestChip8, CycleFetchExec) {
    tempFilePath = "./temp_file.ch8";
    auto testData = generateTempFile(tempFilePath);
    testData.fill(0xFF);
    chip8.LoadROM(tempFilePath);
    
    
    for (int i=1; i<testData.size()/2; i++){
        chip8.Cycle(0);
        ASSERT_EQ(get_opcode(), testData[2*i] << 8u | testData[2*i + 1])
        << "Opcode mismatch at cycle " << i
        << ". Expected: " << std::hex << (testData[2*i] << 8u | testData[2*i + 1]) 
        << ", Got: " << std::hex << get_opcode()
        << std::dec;  // Switch back to decimal for any other comparisons

        ASSERT_EQ(get_pc(), START_ADDRESS + 2 * i)
            << "PC mismatch at cycle " << i
            << ". Expected: " << std::hex << (START_ADDRESS + 2 * i)
            << ", Got: " << std::hex << get_pc()
            << std::dec;
    }
    
    ASSERT_EQ(get_delayTimer(), 0) << "Delay timer was unexpectedly modified.\n";
    ASSERT_EQ(get_soundTimer(), 0) << "Sound timer was unexpectedly modified.\n";
}

TEST_F(TestChip8, CycleTimers) {
    // Setup: Load a known opcode and set timers to a non-zero value
    uint16_t opcode = 0x1234;  // This could be any opcode that doesn't modify the timers
    chip8.memory[0x200] = (opcode >> 8) & 0xFF;
    chip8.memory[0x201] = opcode & 0xFF;
    chip8.pc = 0x200;
    
    // Set the timers to 5 (non-zero values)
    chip8.delayTimer = 5;
    chip8.soundTimer = 5;

    // Call the Cycle function
    chip8.Cycle();

    // Verify that the timers have been decremented by 1
    EXPECT_EQ(chip8.delayTimer, 4) << "Delay timer was not decremented.";
    EXPECT_EQ(chip8.soundTimer, 4) << "Sound timer was not decremented.";
}