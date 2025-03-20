#include "Platform.h"
#include "Tests_common.h"

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