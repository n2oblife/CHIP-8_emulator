#include "../include/Chip8.h"
#include "../include/Chip8_common.h"
#include <cstdint>
#include <fstream>

Chip8::Chip8()
{
    // Initialize the emulator
    pc = START_ADDRESS; // Program counter starts at 0x200
    opcode = 0;         // Reset current opcode
    index = 0;          // Reset index register
    sp = 0;             // Reset stack pointer

    // Clear display, stack, registers, and memory
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++)
        video[i] = 0;
    for (int i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;
    for (int i = 0; i < NUM_REGISTERS; i++)
        registers[i] = 0;
    for (int i = 0; i < MEMORY_SIZE; i++)
        memory[i] = 0;

	// Load fonts into memory
	for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
		memory[FONTSET_START_ADDRESS + i] = fontset[i];

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;

    // Reset keypad state
    for (int i = 0; i < NUM_KEYS; i++)
        keypad[i] = 0;

    // Reset draw flag
    draw_flag = false;
}

// Chip8::~Chip8()
// {
//     // Destructor (if necessary)
// }

void Chip8::LoadROM(char const* filename)
{
	// Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		// Go back to the beginning of the file and fill the buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		// Load the ROM contents into the Chip8's memory, starting at 0x200
		for (long i = 0; i < size; ++i)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}

		// Free the buffer
		delete[] buffer;
	}
}