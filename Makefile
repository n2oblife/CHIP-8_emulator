# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall
LDFLAGS := -L3rdParty/glad -L3rdParty/sdl-2.0.20 -L3rdParty/imgui-1.88
LDLIBS := -lglad -lSDL2 -limgui

# Source files
SOURCES := Source/Chip8.cpp Source/Main.cpp Source/Platform.cpp
OBJECTS := $(SOURCES:.cpp=.o)

# Output binary
TARGET := chip8

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
