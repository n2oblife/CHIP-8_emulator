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

# External libraries
SDL_CFLAGS := $(shell pkg-config --cflags sdl2 2>/dev/null)
SDL_LIBS := $(shell pkg-config --libs sdl2 2>/dev/null)

# If SDL2 is not found, fall back to manual build
ifeq ($(SDL_CFLAGS),)
    SDL_CFLAGS := -I$(LIB_DIR)/SDL2/include
    SDL_LIBS := $(LIB_DIR)/SDL2/libSDL2.a
    BUILD_SDL := yes
endif

# Library includes and linking
INCLUDE_FLAGS := -I$(INCLUDE_DIR) $(SDL_CFLAGS) -I$(LIB_DIR)/imgui
LIBS := $(SDL_LIBS) -lGL -ldl -lpthread

# ImGui source files
IMGUI_DIR := $(LIB_DIR)/imgui
IMGUI_SRCS := $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp \
              $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp \
              $(IMGUI_DIR)/imgui_widgets.cpp $(IMGUI_DIR)/backends/imgui_impl_sdl.cpp \
              $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
IMGUI_OBJS := $(patsubst $(IMGUI_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(IMGUI_SRCS))

# Fetch libraries if needed
.PHONY: fetch
fetch:
	@mkdir -p $(LIB_DIR)
	@if [ "$(BUILD_SDL)" = "yes" ]; then \
	    git clone --depth 1 --branch release-2.0.20 https://github.com/libsdl-org/SDL.git $(LIB_DIR)/SDL2; \
	    cd $(LIB_DIR)/SDL2 && ./configure --prefix=$(LIB_DIR)/SDL2 && make && make install; \
	fi
	@if [ ! -d "$(LIB_DIR)/imgui" ]; then \
	    git clone --depth 1 --branch v1.88 https://github.com/ocornut/imgui.git $(LIB_DIR)/imgui; \
	fi

# Build rules
.PHONY: rules
rules: $(TARGET)

$(TARGET): $(OBJS) $(IMGUI_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Ensure the obj and backend directories exist before compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/backends  # Ensure the backend directory exists
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# Build all executable and fetches if needed
.PHONY: all
all: 
	@make fetch
	@make rules

# Clean build
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Dependency checking
.PHONY: depend
depend:
	@echo "Checking dependencies..."
	@pkg-config --exists sdl2 && echo "SDL2 found!" || echo "SDL2 not found, will build from source."
