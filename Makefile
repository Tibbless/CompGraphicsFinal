# Makefile for Eerie City PS1 Horror Project

CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++11

# Platform-specific settings
UNAME := $(shell uname)

ifeq ($(OS),Windows_NT)
    # Windows (MINGW)
    LDFLAGS = -lfreeglut -lglu32 -lopengl32 -lm
    TARGET = eerie_city.exe
else ifeq ($(UNAME), Darwin)
    # macOS
    LDFLAGS = -framework GLUT -framework OpenGL -framework Cocoa
    TARGET = eerie_city
else
    # Linux
    LDFLAGS = -lglut -lGLU -lGL -lm
    TARGET = eerie_city
endif

SOURCES = main.cpp world_generation.cpp rendering.cpp callbacks.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = eerie_city.h

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

run: $(TARGET)
	./$(TARGET)

# Help target
help:
	@echo "Eerie City - PS1 Horror Aesthetic [Block-Based]"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make clean    - Remove build files"
	@echo "  make run      - Build and run the project"
	@echo "  make help     - Show this help message"
