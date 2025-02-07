# CHIP-8 Emulator

This repository contains a CHIP-8 emulator, a project that emulates the CHIP-8 virtual machine architecture, which was popular in the 1970s and 1980s for programming simple games. This emulator can load and execute CHIP-8 games, providing a window into early low-level programming and emulator development.

The project is based on the work of [Austin Morlan](https://austinmorlan.com/posts/chip8_emulator/) from October 2024, whose guide and codebase were instrumental in building this emulator.

## Table of Contents
- [CHIP-8 Emulator](#chip-8-emulator)
  - [Table of Contents](#table-of-contents)
  - [CHIP-8 Overview](#chip-8-overview)
    - [CHIP-8 Specifications](#chip-8-specifications)
  - [Project Structure](#project-structure)
    - [Key Files](#key-files)
  - [Setup and Installation](#setup-and-installation)
    - [Prerequisites](#prerequisites)
      - [On Ubuntu](#on-ubuntu)
    - [Building the Emulator](#building-the-emulator)
  - [Usage](#usage)
    - [Controls](#controls)
  - [Acknowledgments](#acknowledgments)
  - [License](#license)
    - [Key Elements in This README](#key-elements-in-this-readme)

---

## CHIP-8 Overview
The CHIP-8 is a simple, interpreted programming language used on early computers such as the COSMAC VIP and the Telmac 1800. Although primitive by modern standards, CHIP-8 was designed for writing games, with a limited instruction set that still allows for creative projects.

### CHIP-8 Specifications
- **Memory**: 4KB (4096 bytes)
- **Registers**: 16 general-purpose 8-bit registers
- **Graphics**: Monochrome display with a resolution of 64x32 pixels
- **Input**: 16-key hexadecimal keypad

For more information, see [Austin Morlan's blog](https://austinmorlan.com/posts/chip8_emulator/).

## Project Structure
This repository is organized as follows:

```plaintext
CHIP-8_emulator/
├── include/           # Header files (e.g., Chip8.h, Chip8Constants.h)
├── src/               # Source files (e.g., Chip8.cpp, main.cpp)
├── roms/              # Folder to place CHIP-8 ROM files for testing
├── build/             # Compiled binary and object files (generated by Makefile)
├── Makefile           # Makefile to build the project
└── README.md          # Project documentation
```

### Key Files
- **`Chip8.h`**: Header file defining the `Chip8` class, which contains the core emulator components.
- **`Chip8.cpp`**: Implements the functionality of the `Chip8` class.
- **`main.cpp`**: Entry point for the emulator, initializing and running the emulator loop.
- **`Makefile`**: Build instructions for the project.

## Setup and Installation
This emulator is built using C++17 and requires the SDL2 library for rendering and input handling.

### Prerequisites
- **C++17** compiler (e.g., `g++`)
- **SDL2** library installed on your system

#### On Ubuntu
```bash
sudo apt update
sudo apt install g++ libsdl2-dev make
```

### Building the Emulator
Clone this repository and run the `Makefile` to build the project:

```bash
git clone https://github.com/yourusername/CHIP-8_emulator.git
cd CHIP-8_emulator
make
```

If successful, the compiled binary will be located in `build/bin/chip8_emulator`.

## Usage
Once built, you can load and run a CHIP-8 game ROM by providing the ROM file as a command-line argument:

```bash
./build/bin/chip8_emulator path/to/romfile.ch8
```

### Controls
Most CHIP-8 programs are designed for a 16-key hexadecimal keypad:
```
1 2 3 C
4 5 6 D
7 8 9 E
A 0 B F
```

These keys map to your keyboard, typically as follows:
```
1 2 3 4
Q W E R
A S D F
Z X C V
```

## Acknowledgments
Special thanks to [Austin Morlan](https://austinmorlan.com/posts/chip8_emulator/) for his CHIP-8 emulator guide, which inspired and guided this project.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
```

---

### Key Elements in This README

1. **CHIP-8 Overview**: Explains what CHIP-8 is, including memory, registers, and graphical capabilities.
2. **Project Structure**: Describes how the files and folders are organized.
3. **Setup and Installation**: Provides clear instructions on installing dependencies and building the emulator.
4. **Usage**: Covers running the emulator and describes control mappings.
5. **Acknowledgments**: Gives credit to Austin Morlan's guide, with a link to the source.
6. **License**: Mentions the license (e.g., MIT), which you can add in a `LICENSE` file.