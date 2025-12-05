# Makefile for Final

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Detect operating system
UNAME_S := $(shell uname -s)

# Platform-specific settings
ifeq ($(UNAME_S),Linux)
    # Linux
    LIBS = -lGL -lGLU -lglut -lm
    TARGET = final
endif
ifeq ($(UNAME_S),Darwin)
    # macOS
    LIBS = -framework OpenGL -framework GLUT
    TARGET = final
endif
ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
    # Windows (MINGW)
    LIBS = -lopengl32 -lglu32 -lfreeglut -static
    TARGET = final.exe
endif
ifeq ($(findstring MSYS,$(UNAME_S)),MSYS)
    # Windows (MSYS)
    LIBS = -lopengl32 -lglu32 -lfreeglut -static
    TARGET = final.exe
endif

# Source files
SOURCES = main.cpp callbacks.cpp rendering.cpp world_generation.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files:
HEADERS = eerie_city.h stb_image.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	@echo ""
	@echo "Build successful!"
	@echo "Run with: ./$(TARGET)"

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET) final final.exe

# Rebuild everything
rebuild: clean all

# Install dependencies (Ubuntu/Debian)
install-deps-linux:
	@echo "Installing OpenGL/GLUT dependencies for Linux..."
	sudo apt-get update
	sudo apt-get install -y freeglut3-dev libglu1-mesa-dev mesa-common-dev

# Install dependencies (macOS)
install-deps-mac:
	@echo "OpenGL and GLUT are pre-installed on macOS"
	@echo "If you have issues, try: xcode-select --install"

# Help target
help:
	@echo "Eerie City - Build Commands:"
	@echo "  make              - Build the game (creates 'final' executable)"
	@echo "  make clean        - Remove build files"
	@echo "  make rebuild      - Clean and rebuild"
	@echo "  make install-deps-linux - Install dependencies (Ubuntu/Debian)"
	@echo "  make install-deps-mac   - Install dependencies (macOS)"
	@echo ""
	@echo "After building, run with: ./final"

.PHONY: all clean rebuild install-deps-linux install-deps-mac help
