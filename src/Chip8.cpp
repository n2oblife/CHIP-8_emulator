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
	video.fill(0);
	stack.fill(0);
	memory.fill(0);
	registers.fill(0);

	// Load fonts into memory
	for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
		memory[FONTSET_START_ADDRESS + i] = fontset[i];

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;

    // Reset keypad state
	keypad.fill(0);

    // Reset draw flag
    draw_flag = false;

    // Initialize RNG with uniform distribution for bytes
    randByte = std::uniform_int_distribution<unsigned int>(0, 255U);

	// Set up function pointer table
	InitializeTables();
}

void Chip8::LoadROM(const std::string& filename)
{
    auto buffer = filenameHandling(filename);

    // Load ROM contents into memory starting at 0x200
	std::copy(buffer.begin(), buffer.end(), memory.data() + START_ADDRESS);
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

void Chip8::InitializeTables() {
	// Fill all tables with OP_NULL
	table.fill(&Chip8::OP_NULL);
	table0.fill(&Chip8::OP_NULL);
	table8.fill(&Chip8::OP_NULL);
	tableE.fill(&Chip8::OP_NULL);
	tableF.fill(&Chip8::OP_NULL);

	// Primary opcode table
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

	// Table 0 (Clear and Return)
	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	// Table 8 (Arithmetic)
	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	// Table E (Key handling)
	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;

	// Table F (Timers, memory, and keyboard)
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

std::vector<uint8_t> filenameHandling(const std::string& filename)
{
    if (std::filesystem::path(filename).extension() != ".ch8")
        throw std::runtime_error("The file does not have a .ch8 extension or doesn't exist.\n");

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
        throw std::runtime_error("Failed to open the file, check if it exists or is corrupted\n");

    // Get file size
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > MEMORY_SIZE - START_ADDRESS)
        throw std::runtime_error("The size of the loaded ROM is bigger than actual memory size\n");

    // Read file contents into a vector
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
        throw std::runtime_error("Failed to read the file contents.\n");

    return buffer;
}