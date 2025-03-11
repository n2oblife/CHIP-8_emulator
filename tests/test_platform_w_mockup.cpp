#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Platform.h"
#include <SDL2/SDL.h>

// Mock SDL functions to prevent actual window creation
class MockPlatform : public Platform {
public:
    MockPlatform() : Platform("Test Window", 640, 480, 64, 32) {}

    // Override destructor to avoid SDL cleanup interfering with tests
    ~MockPlatform() {}
};

// Test Fixture
class PlatformTest : public ::testing::Test {
protected:
    MockPlatform* platform;
    uint8_t keys[16] = {0};

    void SetUp() override {
        platform = new MockPlatform();
    }

    void TearDown() override {
        delete platform;
    }
};

// Test SDL Input Processing
TEST_F(PlatformTest, ProcessInput_QuitEvent) {
    // Simulate an SDL_QUIT event
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);

    // The function should return true indicating quit
    EXPECT_TRUE(platform->ProcessInput(keys));
}

TEST_F(PlatformTest, ProcessInput_KeyPress) {
    // Simulate key press (W key -> CHIP-8 key 5)
    SDL_Event event;
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_w;
    SDL_PushEvent(&event);

    platform->ProcessInput(keys);

    // Expect CHIP-8 key 5 to be set to 1
    EXPECT_EQ(keys[5], 1);
}

TEST_F(PlatformTest, ProcessInput_KeyRelease) {
    // Simulate key press and release
    SDL_Event event;
    
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_w;
    SDL_PushEvent(&event);

    platform->ProcessInput(keys);
    EXPECT_EQ(keys[5], 1);  // Key should be pressed

    event.type = SDL_KEYUP;
    SDL_PushEvent(&event);

    platform->ProcessInput(keys);
    EXPECT_EQ(keys[5], 0);  // Key should be released
}

