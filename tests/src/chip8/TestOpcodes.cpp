#include <TestChip8.h>

// TODO: finish the opcode tests

TEST_F(TestChip8, OP_00E0) {
    get_video().fill(1);
    chip8.OP_00E0();
    std::array<uint32_t, VIDEO_HEIGHT*VIDEO_WIDTH> video_null = {0};
    ASSERT_EQ(get_video(), video_null) << "OP_00E0 didn't reset the video display";
}


TEST_F(TestChip8, OP_00EE) {
    auto pc_test = generateRandomInt<uint16_t>(std::numeric_limits<uint16_t>::max());
    auto sp_test = generateRandomInt(STACK_SIZE);
    get_sp() = sp_test;
    get_stack().at(get_sp()) = pc_test;

    chip8.OP_00EE();

    ASSERT_EQ(get_sp(), sp_test-1) << "OP_00EE didn't decrement by 1";
    ASSERT_EQ(get_pc(), pc_test) << "OP_00EE didn't load the right value from stack";
}

// TEST_F(TestChip8, OP_1nnn) {

// }

// TEST_F(TestChip8, OP_2nnn) {

// }

// TEST_F(TestChip8, OP_3xkk) {

// }

// TEST_F(TestChip8, OP_4xkk) {

// }

// TEST_F(TestChip8, OP_5xy0) {

// }

// TEST_F(TestChip8, OP_6xkk) {

// }

// TEST_F(TestChip8, OP_7xkk) {

// }

// TEST_F(TestChip8, OP_8xy0) {

// }

// TEST_F(TestChip8, OP_8xy1) {

// }

// TEST_F(TestChip8, OP_8xy2) {

// }

// TEST_F(TestChip8, OP_8xy3) {

// }

// TEST_F(TestChip8, OP_8xy4) {

// }

// TEST_F(TestChip8, OP_8xy5) {

// }

// TEST_F(TestChip8, OP_8xy6) {

// }

// TEST_F(TestChip8, OP_8xy7) {

// }

// TEST_F(TestChip8, OP_8xyE) {

// }

// TEST_F(TestChip8, OP_9xy0) {

// }

// TEST_F(TestChip8, OP_Annn) {

// }

// TEST_F(TestChip8, OP_Bnnn) {

// }

// TEST_F(TestChip8, OP_Cxkk) {

// }

// TEST_F(TestChip8, OP_Dxyn) {

// }

// TEST_F(TestChip8, OP_Ex9E) {

// }

// TEST_F(TestChip8, OP_ExA1) {

// }

// TEST_F(TestChip8, OP_Fx07) {

// }

// TEST_F(TestChip8, OP_Fx0A) {

// }

// TEST_F(TestChip8, OP_Fx0A_optimized) {

// }

// TEST_F(TestChip8, OP_Fx15) {

// }

// TEST_F(TestChip8, OP_Fx18) {

// }

// TEST_F(TestChip8, OP_Fx1E) {

// }

// TEST_F(TestChip8, OP_Fx29) {

// }

// TEST_F(TestChip8, OP_Fx33) {

// }

// TEST_F(TestChip8, OP_Fx55) {

// }

// TEST_F(TestChip8, OP_Fx65) {

// }

// TEST_F(TestChip8, OP_NULL) {

// }
