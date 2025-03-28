# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall

# Directories
PROJECT_DIR := $(shell pwd)
SRC_DIR := $(PROJECT_DIR)/src
INCLUDE_DIR := $(PROJECT_DIR)/include
BUILD_DIR := $(PROJECT_DIR)/build
OBJ_DIR := $(BUILD_DIR)/obj
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Executable name
TARGET := $(BIN_DIR)/chip8
TARGET_TESTS := $(BIN_DIR)/tests/chip8_tests

# Reports
REPORT_NAME = reports
EXTENSION = xml #xml or json
# EXTENSION = $(patsubst .%,%,$(suffix $(REPORT)))
LOG_DIR := $(PROJECT_DIR)/log
TIMESTAMP = $(shell date +'%Y%m%d_%H%M%S')  # Current timestamp
COMMIT_HASH = $(shell git rev-parse --short HEAD)  # Git commit hash
# REPORT = $(REPORT_NAME)_$(COMMIT_HASH)_$(TIMESTAMP).$(EXTENSION)
REPORT = $(LOG_DIR)/$(REPORT_NAME)_$(strip $(COMMIT_HASH))_$(strip $(TIMESTAMP)).$(EXTENSION)

# Default build type
BUILD_TYPE := Release

.PHONY: all
all:
	@echo "====== Starting the build process... ======"
	@make depend BUILD_TYPE=$(BUILD_TYPE)
	@make build BUILD_TYPE=$(BUILD_TYPE)

.PHONY: target
target:
	@echo "====== Starting the build process... ======"
	@make depend BUILD_TYPE=$(BUILD_TYPE)
	@make build $(wordlist 2, $(words $(MAKECMDGOALS)), $(MAKECMDGOALS)) BUILD_TYPE=$(BUILD_TYPE)

.PHONY: depend
depend:
	@echo "====== Configuring project with CMake... ======"
	@cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -S $(PROJECT_DIR) -B $(BUILD_DIR)

.PHONY: build
build:
	@echo "====== Building project... ======"
	@cmake --build $(BUILD_DIR) -j$(nproc) --target $(wordlist 2, $(words $(MAKECMDGOALS)), $(MAKECMDGOALS))
	@echo "====== Build complete! ======"

.PHONY: clean
clean:
	@echo "====== Cleaning up build and report files... ======"
	@rm -rf $(BUILD_DIR)
	@rm -rf $(LOG_DIR)
	@rm -rf test_detail.xml
	@rm -rf test_detail.json
	@echo "====== Clean up complete! ======"

.PHONY: tests
tests:
	@echo "====== Running tests... ======"
	@echo "Log saved in $(REPORT)"
	@$(TARGET_TESTS) --gtest_output=$(strip $(EXTENSION)):$(REPORT)
	@echo "====== Tests complete! ======"

.PHONY: run
run:
	@if [ ! -f "${TARGET}" ]; then \
		echo "====== Error: '${TARGET}' not found! Build the project first. ======"; \
		exit 1; \
	fi;
	@ROM_FILE=$(word 2, $(MAKECMDGOALS)); \
	if echo "$$ROM_FILE" | grep -Eqs "^(http|https)://"; then \
		echo "====== URL detected, downloading ROM: $$ROM_FILE ======"; \
		FILE_NAME=$$(basename "$$ROM_FILE"); \
		wget -O "$$FILE_NAME" "$$ROM_FILE"; \
		ROM_FILE="$$FILE_NAME"; \
	fi; \
	echo "====== Launching emulator with ROM: $$ROM_FILE ======"; \
	${TARGET} "$$ROM_FILE"; \
	echo "====== Emulator terminated! ======"

# Allow running with `make run <ROM>`
%:
	@:

.PHONY: doc
doc:
	@echo "====== Generating Documentation... ======"
	@cmake --build build --target doc
	@cd docs/latex && pdflatex refman.tex && mv refman.pdf ../chip8_doc.pdf && rm -rf ../latex/
	@echo "====== Documentation Generated in build/docs ======"


.PHONY: graph
graph:
	@if [ ! -f "${TARGET}" ]; then \
		echo "====== Error: '${TARGET}' not found! Build the project first. ======"; \
		exit 1; \
	fi;
	@echo "====== Generating CMake graph... ======"; \
	mkdir -p $(BUILD_DIR)/graph; \
	cd $(BUILD_DIR) && cmake --graphviz=graph/project.dot $(PROJECT_DIR)
	@dot -Tsvg $(BUILD_DIR)/graph/project.dot -o $(BUILD_DIR)/graph/project.svg; \
	echo "====== Graph generated: $(BUILD_DIR)/graph/project.svg ======"


.PHONY: help
help:
	@echo "Usage: make [target_name] [options]"
	@echo ""
	@echo "Available targets:"
	@echo "  all [Release|Debug|MinSizeRel]   - Configure and build the entire project (default: Release)."
	@echo "  target [target_name]             - Build a specific target after configuring the project."
	@echo "  depend [Release|Debug|MinSizeRel]- Run CMake configuration only."
	@echo "  build [target_name]              - Build the project or a specific target using CMake."
	@echo "  clean                            - Remove build files, logs, and test reports."
	@echo "  tests                            - Run unit tests using GoogleTest and save logs."
	@echo "  run <ROM>                        - Launch the emulator with a specified ROM file or its URL."
	@echo "  doc                        - Launch the emulator with a specified ROM file or its URL."
	@echo "  graph                            - Generate a CMake dependency graph for visualization."
	@echo "  help                             - Display this help message."
	@echo ""
	@echo "Options:"
	@echo "  BUILD_TYPE=[Release|Debug|MinSizeRel]  - Specify the build type for 'all' or 'depend' (default: Release)."
	@echo "  [target_name]                          - Specify a particular target to build (default: all, e.g., 'chip8' or 'chip8_tests')."
	@echo "  REPORT_NAME=<name_of_report>           - Set a custom name for the test report file (default: report)."
	@echo "  EXTENSION=[xml|json]                   - Choose the format of the test report output (default: xml)."
	@echo ""
	@echo "Examples:"
	@echo "  make all Debug                     - Configure and build the project in Debug mode."
	@echo "  make target chip8                   - Build only the 'chip8' target."
	@echo "  make build chip8_tests              - Build only the 'chip8_tests' target."
	@echo "  make run test_opcode.ch8            - Run the emulator with 'test_opcode.ch8'."
	@echo "  make tests                          - Run all tests and generate a report."
	@echo "  make graph                          - Generate a project dependency graph."
	@echo "  make clean                          - Clean up build artifacts and logs."


