#include "Platform.h"
#include "Tests_common.h"
// TODO: change the functions with keyup and down
class TestPlatform : public  ::testing::Test
{
protected:
    Platform platform = Platform("Test", 640, 320, 64, 32);
    uint8_t keys[NUM_KEYS] = {0};
    SDL_Event event;

    void SetUp() override {}

    void TearDown() override {}

    inline void KeyDown(SDL_Event& event, const int key) {
        event.type = SDL_KEYDOWN;
        event.key.keysym.sym = key;
        SDL_PushEvent(&event);  // Push the event into SDL's queue
    }

    inline void KeyUp(SDL_Event& event) {
        event.type = SDL_KEYUP;
        SDL_PushEvent(&event);
    }
};

// ====== Testing base class functions ======

// This test checks whether the Platform object can be created and destroyed without crashing.
TEST_F(TestPlatform, ConstructorDestructor) {
    Platform platform("Test", 640, 320, 64, 32);
}

// ====== Testing input handling ======

TEST_F(TestPlatform, ProcessInputKeyDown) {
    KeyDown(event, (int)'x');
    platform.ProcessInput(keys);

    // Key x should be marked as pressed and no other
    EXPECT_EQ(keys[0], 1) << "key x is not considered pressed";
    // Failing test to check
    // EXPECT_EQ(keys[0], 0) << "key x is not considered pressed";
    for (int i=1; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed"; 
}

TEST_F(TestPlatform, ProcessInputKeyUp) {
    KeyDown(event, (int)'x');
    platform.ProcessInput(keys);

    EXPECT_EQ(keys[0], 1) << "key x is not considered pressed";
    for (int i=1; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed";

    KeyUp(event);    
    platform.ProcessInput(keys);

    for (int i=0; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed";
}


TEST_F(TestPlatform, ProcessInputKeyOutBound) {
    // 'b' is not handled by the emulator
    KeyDown(event, (int)'b');
    platform.ProcessInput(keys);

    for (int i=0; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed";

    KeyUp(event);
    platform.ProcessInput(keys);

    for (int i=0; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed";
}

TEST_F(TestPlatform, ProcessInputQuitEvent) {
    // Simulate SDL_QUIT event
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);

    bool quit = platform.ProcessInput(keys);
    EXPECT_TRUE(quit) << "Platform::ProcessInput didn't request quit";
}

// TODO: improve testing with some random data

// ====== Testing Update ======

TEST_F(TestPlatform, UpdateRunsWithoutCrash) {
    // Create a dummy buffer
    uint32_t buffer[64 * 32] = {0};
    platform.Update(buffer, 64 * sizeof(uint32_t));

    // TODO: check value in memory to ensure correct update
    SUCCEED() << "Platform::Update crashed";
}