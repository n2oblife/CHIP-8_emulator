#include "Chip8.h"


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

	/*
	----- Critical thinking -----
	Goal : execute opcode and link it to the functions
	- Create a table of function pointers to the opcode functions
		-> pros : 
		- Directly indexing into an array is faster than repeatedly evaluating conditions
		- Easier to read and maintain
		-> cons :
		- Need to create a function pointer for each opcode (can be slow)
		- Memory overhead for the function pointers when there is no defined opcode
	- Create switch statements to link the opcode to the function
		-> pros :
		- Easier to implement new functions
		- No memory overhead for undefined opcodes
		-> cons :
		- Slower than function pointers but no nead to create pointers
		- each case evaluated one by one => slow
	- Create hash tables to link the opcode to the function
		-> pros :
		- Dynamic allocation only for used opcodes, reducing memory overhead
		-> cons :
		- Map lookups are slower than direct indexing and add complexity
	- Create Jump table with goto labels to link the opcode to the function
		-> pros :
		- Jump tables can be extremely fast, as each opcode directly jumps to a memory address, similar to switch statements optimized by compilers
		-> cons :
		- goto-based jump tables are less readable, harder to maintain, and platform-dependent
	*/

	// Set up function pointer table
	table[0x0] = &Chip8::Table0;
	table[0x1] = &Chip8::OP_1nnn;
	table[0x2] = &Chip8::OP_2nnn;
	table[0x3] = &Chip8::OP_3xkk;
	table[0x4] = &Chip8::OP_4xkk;
	table[0x5] = &Chip8::OP_5xy0;
	table[0x6] = &Chip8::OP_6xkk;
	table[0x7] = &Chip8::OP_7xkk;
	table[0x8] = &Chip8::Table8;
	table[0x9] = &Chip8::OP_9xy0;
	table[0xA] = &Chip8::OP_Annn;
	table[0xB] = &Chip8::OP_Bnnn;
	table[0xC] = &Chip8::OP_Cxkk;
	table[0xD] = &Chip8::OP_Dxyn;
	table[0xE] = &Chip8::TableE;
	table[0xF] = &Chip8::TableF;

	/*
	Set all unimplemented opcodes to NULL function pointer 
	to prevent crashes when executing from uninitialized memory
	*/
	for (size_t i = 0; i <= 0xE; i++)
	{
		table0[i] = &Chip8::OP_NULL;
		table8[i] = &Chip8::OP_NULL;
		tableE[i] = &Chip8::OP_NULL;
	}
	for (size_t i = 0; i <= 0x65; i++)
	{
		tableF[i] = &Chip8::OP_NULL;
	}

	// even with E0 and EE function, only difference is last hex digit
	// hence opcode table can be limited (still overhead for unused opcodes)
	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	// even with A1 and 9E function, only difference is last hex digit
	// hence opcode table can be limited (still overhead for unused opcodes)
	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;

	tableF[0x07] = &Chip8::OP_Fx07;
	tableF[0x0A] = &Chip8::OP_Fx0A;
	tableF[0x15] = &Chip8::OP_Fx15;
	tableF[0x18] = &Chip8::OP_Fx18;
	tableF[0x1E] = &Chip8::OP_Fx1E;
	tableF[0x29] = &Chip8::OP_Fx29;
	tableF[0x33] = &Chip8::OP_Fx33;
	tableF[0x55] = &Chip8::OP_Fx55;
	tableF[0x65] = &Chip8::OP_Fx65;
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

void Chip8::Cycle()
{
	// Fetch
	// memory[pc] << 8u : turn this into a 16-bit value
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	// Increment the PC before we execute anything
	pc += 2;

	// Decode and Execute based on the opcode
	((*this).*(table[(opcode & 0xF000u) >> 12u]))();

	// Decrement the delay timer if it's been set
	if (delayTimer > 0)
		--delayTimer;

	// Decrement the sound timer if it's been set
	if (soundTimer > 0)
		--soundTimer;
}