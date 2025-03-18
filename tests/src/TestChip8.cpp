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
};

// ====== Testing base class functions ======

// This test checks whether the Platform object can be created and destroyed without crashing.
TEST_F(TestChip8, ConstructorDestructor) {
    Chip8 chip8;
}

TEST_F(TestChip8, GetVideo) {
    auto testData = generateRandomData<uint32_t>(VIDEO_WIDTH * VIDEO_HEIGHT);
    std::copy(testData.begin(), testData.end(), get_video().begin());  // Fill the video array
    ASSERT_EQ(chip8.get_video(), testData) << "The video geter is broken\n";
}

TEST_F(TestChip8, GetKeypad) {
    auto testData = generateRandomData<uint8_t>(NUM_KEYS);
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

TEST_F(TestChip8, LoadingRom) {
    tempFilePath = "./temp_file.ch8";
    auto testData = generateTempFile(tempFilePath);
    ASSERT_NO_THROW(chip8.LoadROM(tempFilePath)) << "Chip8 didn't handle the file extension\n";
    // ASSERT_EQ()
    std::filesystem::remove(tempFilePath);
}