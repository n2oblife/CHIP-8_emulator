// #include "chip8/TestChip8.h"
#include <TestChip8.h>

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

// ====== Testing Cycle function ======

TEST_F(TestChip8, CycleFetchExec) {
    tempFilePath = "./temp_file.ch8";
    auto testData = generateTempFile(tempFilePath);
    testData.fill(0xFF);
    chip8.LoadROM(tempFilePath);
    
    
    for (int i=1; i<testData.size()/2; i++){
        chip8.Cycle();
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

// TEST_F(TestChip8, TableCall) {
//     tempFilePath = "./temp_file.ch8";
//     std::array<uint8_t, 2> testData = {0x00, 0x01};
//     writeFile(tempFilePath, testData);    
//     chip8.LoadROM(tempFilePath); 

//     // TODO: check correct function is called through table system 
//     // I think it's broken
// }

TEST_F(TestChip8, CycleTimers) {
    tempFilePath = "./temp_file.ch8";
    std::array<uint8_t, 8> testData;
    testData.fill(0x00);  // Changed to a valid NOP-equivalent opcode
    for (size_t i = 1; i < testData.size(); i += 2) {
        testData[i] = 0x01; // Second byte of opcode
    }
    writeFile(tempFilePath, testData);    
    chip8.LoadROM(tempFilePath);
    
    // Set the timers to a non-zero values
    get_delayTimer() = testData.size()/2;
    get_soundTimer() = testData.size()/2;

    for (int i=1; i<testData.size()/2; i++){
        chip8.Cycle();

        ASSERT_EQ(get_delayTimer(), testData.size()/2-i) << "Delay timer was not decremented.";
        ASSERT_EQ(get_soundTimer(), testData.size()/2-i) << "Sound timer was not decremented.";
    }
}