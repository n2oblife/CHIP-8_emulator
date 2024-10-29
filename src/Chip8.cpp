#include "../include/Chip8.h"
#include "../include/Chip8_common.h"
#include <cstdint>
#include <fstream>
#include <chrono>


Chip8::Chip8()
    :randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    // Initialize the emulator
    pc = START_ADDRESS; // Program counter starts at 0x200
    opcode = 0;         // Reset current opcode
    index = 0;          // Reset index register
    sp = 0;             // Reset stack pointer

    // Clear display, stack, registers, and memory by setting value to 0
    memset(video, 0, sizeof(video));
    memset(stack, 0, sizeof(stack));
    memset(memory, 0, sizeof(memory));
    memset(registers, 0, sizeof(registers));

	// Load fonts into memory
	for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
		memory[FONTSET_START_ADDRESS + i] = fontset[i];

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;

    // Reset keypad state
    memset(keypad, 0, sizeof(keypad));

    // Reset draw flag
    draw_flag = false;

    // Initialize RNG with uniform distribution for bytes
    randByte = std::uniform_int_distribution<unsigned int>(0, 255U);
}

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
			memory[START_ADDRESS + i] = buffer[i];

		// Free the buffer
		delete[] buffer;
	}
}

void Chip8::OP_00E0()
{
	memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE()
{
	--sp;
	pc = stack[sp];
}

void Chip8::OP_1nnn()
{
	uint16_t address = opcode & 0x0FFFu;

	pc = address;
}

void Chip8::OP_2nnn()
{
	uint16_t address = opcode & 0x0FFFu;

	stack[sp] = pc;
	++sp;
	pc = address;
}

void Chip8::OP_3xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] == byte)
		pc += 2;
}

void Chip8::OP_4xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] != byte)
		pc += 2;
}

void Chip8::OP_5xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] == registers[Vy])
	{
		pc += 2;
	}
}

void Chip8::OP_6xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = byte;
}

void Chip8::OP_7xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] += byte;
}