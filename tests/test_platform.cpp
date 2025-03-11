#include "Platform.h"
#include <gtest/gtest.h>
#include <SDL2/SDL.h>

// TODO: should look at formal testing to handle all the cases

// ====== Testing base class functions ======

// This test checks whether the Platform object can be created and destroyed without crashing.
TEST(PlatformTest, ConstructorDestructor) {
    Platform platform("Test", 640, 320, 64, 32);
}

// ====== Testing input handling ======

TEST(PlatformTest, ProcessInputKeyDown) {
    Platform platform("Test", 640, 320, 64, 32);
    uint8_t keys[NUM_KEYS] = {0};

    // Simulate a key press event for key 'x'
    SDL_Event event;
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_x;
    SDL_PushEvent(&event);  // Push the event into SDL's queue

    platform.ProcessInput(keys);

    // Key x should be marked as pressed and no other
    EXPECT_EQ(keys[0], 1) << "key x is not considered pressed";
    // Failing test to check
    // EXPECT_EQ(keys[0], 0) << "key x is not considered pressed";
    for (int i=1; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed"; 
}

TEST(PlatformTest, ProcessInputKeyUp) {
    Platform platform("Test", 640, 320, 64, 32);
    uint8_t keys[NUM_KEYS] = {0};

    // Simulate a key press and release event for key 'x'
    SDL_Event event;
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_x;
    SDL_PushEvent(&event);

    platform.ProcessInput(keys);

    EXPECT_EQ(keys[0], 1) << "key x is not considered pressed";
    for (int i=1; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed";

    event.type = SDL_KEYUP;
    SDL_PushEvent(&event);
    
    platform.ProcessInput(keys);

    for (int i=0; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed";
}


TEST(PlatformTest, ProcessInputKeyOutBound) {
    Platform platform("Test", 640, 320, 64, 32);
    uint8_t keys[NUM_KEYS] = {0};

    // Simulate a key press and release event for key 'b'
    // this key is not handled by the emulator
    SDL_Event event;
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_b;
    SDL_PushEvent(&event);

    platform.ProcessInput(keys);
    for (int i=0; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed";

    event.type = SDL_KEYUP;
    SDL_PushEvent(&event);
    
    platform.ProcessInput(keys);
    for (int i=0; i<NUM_KEYS; i++)
        EXPECT_EQ(keys[i], 0) << "key " << i << " is considered pressed";
}

TEST(PlatformTest, ProcessInputQuitEvent) {
    Platform platform("Test", 640, 320, 64, 32);
    uint8_t keys[NUM_KEYS] = {0};

    // Simulate SDL_QUIT event
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);

    bool quit = platform.ProcessInput(keys);
    EXPECT_TRUE(quit) << "Platform::ProcessInput didn't request quit";
}


// ====== Testing Update ======

TEST(PlatformTest, UpdateRunsWithoutCrash) {
    Platform platform("Test", 640, 320, 64, 32);

    // Create a dummy buffer
    uint32_t buffer[64 * 32] = {0};
    platform.Update(buffer, 64 * sizeof(uint32_t));

    // TODO: check value in memory to ensure correct update
    SUCCEED() << "Platform::Update crashed";
}