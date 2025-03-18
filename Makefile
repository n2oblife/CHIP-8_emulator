# TODO: handle tests and library building
# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Executable name
TARGET := $(BIN_DIR)/chip8
TARGET_TESTS := $(BIN_DIR)/chip8_tests

# Reports
REPORT_PREFIX = log/reports
EXTENSION = xml #xml or json
# EXTENSION = $(patsubst .%,%,$(suffix $(REPORT)))
TIMESTAMP = $(shell date +'%Y%m%d_%H%M%S')  # Current timestamp
COMMIT_HASH = $(shell git rev-parse --short HEAD)  # Git commit hash
# REPORT = $(REPORT_PREFIX)_$(COMMIT_HASH)_$(TIMESTAMP).$(EXTENSION)
REPORT = $(REPORT_PREFIX)_$(strip $(COMMIT_HASH))_$(strip $(TIMESTAMP)).$(EXTENSION)


.PHONY: all
all:
	@echo "====== Configuring project with CMake... ======"
	@cmake -S . -B build
	@echo "====== Building project... ======"
	@cmake --build build
	@echo "====== Build complete! ======"

.PHONY: depend
depend:
	@echo "====== Configuring project with CMake... ======"
	@cmake -S . -B build

.PHONY: build
build:
	@echo "====== Building project... ======"
	@cmake --build build
	@echo "====== Build complete! ======"

.PHONY: clean
clean:
	@echo "====== Cleaning up build and report files... ======"
	@rm -rf $(BUILD_DIR)
	@rm -rf $(REPORT)
	@rm -rf test_detail.xml
	@rm -rf test_detail.json
	@echo "====== Clean up complete! ======"

.PHONY: tests
tests:
	@echo "====== Running tests... ======"
	$(TARGET_TESTS) --gtest_output=$(strip $(EXTENSION)):$(REPORT)
	@echo "Results saved in $(REPORT)"
	@echo "====== Tests complete! ======"

# TODO: not finished, need to download rom and load it
.PHONY: run
run:
	@echo "====== Launching target with ROM file: $(word 2, $(MAKECMDGOALS)) ======"
	@${TARGET} $(word 2, $(MAKECMDGOALS))
	@echo "====== Launch complete! ======"
