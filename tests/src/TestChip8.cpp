#include "Chip8.h"
#include "Tests_common.h"

class TestChip8 : public  ::testing::Test
{
protected:
    Chip8 chip8;
    uint8_t keys[NUM_KEYS] = {0};
    SDL_Event event;

    void SetUp() override {}

    void TearDown() override {}
};

// ====== Testing base class functions ======

// This test checks whether the Platform object can be created and destroyed without crashing.
TEST_F(TestChip8, ConstructorDestructor) {
    Chip8 chip8;
}

TEST_F(TestChip8, GetVideo) {
    auto testData = generateRandomData<uint32_t>(VIDEO_WIDTH * VIDEO_HEIGHT);
    std::copy(testData.begin(), testData.end(), chip8.video);
    ASSERT_EQ(chip8.get_video(), testData) << "The video geter is broken\n";
}

TEST_F(TestChip8, GetKeypad) {
    auto testData = generateRandomData<uint8_t>(NUM_KEYS);
    std::copy(testData.begin(), testData.end(), chip8.keypad);
    ASSERT_EQ(chip8.get_keypad(), testData) << "The keypad geter is broken\n";
}

// ====== Testing loading ROM ======

TEST_F(TestChip8, EmptyFilenameError) {
    ASSERT_THROW(chip8.LoadROM('')) << "Chip8 loaded an empty filename\n";
}

TEST_F(TestChip8, ExtensionError) {
    ASSERT_THROW(chip8.LoadROM('just/a/file.test')) << "Chip8 didn't handle the file extension\n";
}

TEST_F(TestChip8, LoadsROM) {
    ASSERT_NO_THROW(chip8.LoadROM('')) << "Chip8 could not load the ROM correctly (extension or size issue)\n";

}