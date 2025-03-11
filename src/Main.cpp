#include <iostream>
#include "Chip8.h"
#include "Platform.h"

int main(int argc, char** argv)
{
	// Declare variables by default in main scope
	const char* romFilename = nullptr;
	int videoScale = DEFAULT_VIDEO_SCALE;
	int cycleDelay = DEFAULT_CYCLE_DELAY;

	// Default values with only RomFile
	if (argc == 2)
	{
		romFilename = argv[1];
	}
	else if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <ROM> [Scale] [Delay]\n";
		std::exit(EXIT_FAILURE);
	}
	else
	{
		romFilename = argv[1];
		videoScale = std::stoi(argv[2]);
		cycleDelay = std::stoi(argv[3]);
	}

	// Make sure that romFilename is not null
	if (!romFilename)
	{
		std::cerr << "Error: ROM filename is required.\n";
		std::exit(EXIT_FAILURE);
	}


	Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

	Chip8 chip8;
	chip8.LoadROM(romFilename);

	int videoPitch = sizeof(chip8.get_keypad()[0]) * VIDEO_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = platform.ProcessInput(chip8.get_keypad());

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			chip8.Cycle();

			platform.Update(chip8.get_video(), videoPitch);
		}
	}

	return 0;
}