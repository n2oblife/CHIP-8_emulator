#include "Chip8.h"
//TODO: adapt this file to C++ standards with std::arrays instead of raw arrays

void Chip8::OP_00E0()
{
	video.fill(0);
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
		pc += 2;
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

void Chip8::OP_8xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] = registers[Vy];
}

void Chip8::OP_8xy1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] |= registers[Vy];
}

void Chip8::OP_8xy2()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] &= registers[Vy];
}

void Chip8::OP_8xy3()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] ^= registers[Vy];
}

void Chip8::OP_8xy4()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = registers[Vx] + registers[Vy];

	if (sum > 255U)
		registers[0xF] = 1;
	else
		registers[0xF] = 0;

	registers[Vx] = sum & 0xFFu;
}

void Chip8::OP_8xy5()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] > registers[Vy])
		registers[0xF] = 1;
	else
		registers[0xF] = 0;

	registers[Vx] -= registers[Vy];
}


void Chip8::OP_8xy6()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	// save the least significant bit in VF
	registers[0xF] = (registers[Vx] & 0x1u);
	registers[Vx] >>= 1;
}

void Chip8::OP_8xy7()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vy] > registers[Vx])
		registers[0xF] = 1;
	else
		registers[0xF] = 0;

	registers[Vx] = registers[Vy] - registers[Vx];
}

void Chip8::OP_8xyE()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	// save the most significant bit in VF
	registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

	registers[Vx] <<= 1;
}

void Chip8::OP_9xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] != registers[Vy])
		pc += 2;
}

void Chip8::OP_Annn()
{
	uint16_t address = opcode & 0x0FFFu;
	index = opcode & 0x0FFFu;
}

void Chip8::OP_Bnnn()
{
	uint16_t address = opcode & 0x0FFFu;
	pc = registers[0] + address;
}

void Chip8::OP_Cxkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = randByte(randGen) & byte;
}

void Chip8::OP_Dxyn()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries (modulo to fit)
	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		// Fetch the current sprite row
		// index supposed to be already set to the address in memory
		uint8_t spriteByte = memory[index + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			// Check if sprite pixel is active
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on -> collision
				if (*screenPixel == 0xFFFFFFFF)
					registers[0xF] = 1; // Collision detected flag

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF; // Toggle the screen pixel on/off
			}
		}
	}
}

void Chip8::OP_Ex9E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = registers[Vx];

	if (keypad[key])
		pc += 2;
}

void Chip8::OP_ExA1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = registers[Vx];

	if (!keypad[key])
		pc += 2;
}

void Chip8::OP_Fx07()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	registers[Vx] = delayTimer;
}

void Chip8::OP_Fx0A()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    // Iterate through the keypad to find a pressed key
    for (uint8_t i = 0; i < NUM_KEYS; ++i) {
        if (keypad[i]) {
            registers[Vx] = i;  // Store the key pressed in Vx
            return;             // Exit the function as we've found a key
        }
    }

    // No key was pressed, so we decrement the program counter to wait
    pc -= 2;
}

void Chip8::OP_Fx0A_optimized()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    /*
	Generate a bitmask of pressed keys.
	This part is optimized by modern compilers
	and tailored for the CHIP-8 keypad
	*/
    uint16_t keyMask = 0;
    for (uint8_t i = 0; i < 16; ++i) {
        if (keypad[i]) {
            keyMask |= (1 << i);
        }
    }

    if (keyMask) {
        // Find the lowest set bit, indicating the first key pressed
        registers[Vx] = __builtin_ctz(keyMask);  // GCC/Clang intrinsic
    } else {
        pc -= 2;  // No key pressed, wait by decrementing PC
    }
}

void Chip8::OP_Fx15()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = registers[Vx];
}

void Chip8::OP_Fx18()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = registers[Vx];
}

void Chip8::OP_Fx1E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	index += registers[Vx];
}

void Chip8::OP_Fx29()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = registers[Vx];

	index = FONTSET_START_ADDRESS + (FONT_SIZE * digit);
}

void Chip8::OP_Fx33()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; 	// Extract Vx register
    uint8_t value = registers[Vx];         	// Get the value stored in Vx

	// Not using a loop enable the compiler to optimize the code
    memory[index + 2] = value % 10;   		// Store the ones digit in I+2
    value /= 10;                      		// Remove the ones digit

    memory[index + 1] = value % 10;   		// Store the tens digit in I+1
    value /= 10;                      		// Remove the tens digit

    memory[index] = value % 10;       		// Store the hundreds digit in I
}

void Chip8::OP_Fx55()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
		memory[index + i] = registers[i];
}

void Chip8::OP_Fx65()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
		registers[i] = memory[index + i];
}

void Chip8::Table0()
{
	((*this).*(table0[opcode & 0x000Fu]))();
};

void Chip8::Table8()
{
	((*this).*(table8[opcode & 0x000Fu]))();
};

void Chip8::TableE()
{
	((*this).*(tableE[opcode & 0x000Fu]))();
};

void Chip8::TableF()
{
	((*this).*(tableF[opcode & 0x00FFu]))();
};
