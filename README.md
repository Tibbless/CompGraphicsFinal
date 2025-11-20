# Eerie City - PS1 Horror Aesthetic

A procedurally generated PS1-style horror city with authentic retro visual effects including vertex jitter, dithering, heavy fog, and atmospheric lighting.

## Features

- **Block-based city generation** with 9x9 grid system
- **Multiple block types**: Buildings, parks, industrial zones, and graveyards
- **Atmospheric lighting**: Street lamps, moonlight, and player light with dynamic flicker
- **PS1 visual effects**: Vertex jitter, dithering, low polygon aesthetic
- **Varied tree types**: Layered, dead, and twisted trees for horror atmosphere
- **Heavy fog system** for limited visibility (~50ft range)

## Requirements

### Linux
- g++ compiler
- OpenGL libraries (`libGL`, `libGLU`)
- GLUT library (`libglut`)

### macOS
- Xcode Command Line Tools
- OpenGL and GLUT frameworks (included with macOS)

### Windows (MINGW64)
- MinGW-w64
- FreeGLUT library
- OpenGL libraries

## Building

### Linux
```bash
make
```

### macOS
```bash
make
```

### Windows (MINGW64)
```bash
make
```

The Makefile automatically detects your platform and uses the appropriate compiler flags.

## Running

After building, run the executable:

### Linux/macOS
```bash
./eerie_city
```

### Windows
```bash
./eerie_city.exe
```

Or simply use:
```bash
make run
```

## Controls

### Movement
- **WASD** / **Arrow Keys** - Move forward/backward and turn left/right
- **Q/E** - Strafe left/right
- **Z/X** - Look up/down

### Atmosphere
- **T** - Toggle automatic time advancement
- **N/M** - Increase/decrease noise effect
- **F/G** - Increase/decrease light flicker intensity
- **Shift+D** - Toggle dither effect

### Other
- **R** - Reset player position to origin
- **ESC** - Exit

## Project Structure

```
eerie_city/
├── main.cpp              # Main program and initialization
├── world_generation.cpp  # City block and object generation
├── rendering.cpp         # OpenGL drawing functions
├── callbacks.cpp         # Input handling and display
├── eerie_city.h          # Header with structure definitions
└── Makefile             # Cross-platform build configuration
```

## Technical Details

- **City Grid**: 9×9 blocks (81 total blocks)
- **Block Size**: 30 units with 10-unit roads
- **Block Distribution**: 55% buildings, 20% parks, 15% industrial, 10% graveyards
- **Lighting System**: Uses up to 8 OpenGL lights (moonlight, 6 street lamps, player light)
- **Fog Density**: 0.025 for ~15 unit (~50ft) visibility
- **Window Probability**: 95% of buildings have lit windows

## Cleaning Build Files

```bash
make clean
```

## Help

```bash
make help
```

## Known Issues

- Polygon offset used to prevent Z-fighting between roads and edges
- Some platforms may require adjustment of fog density for performance
- Player light provides strong illumination in immediate vicinity

## What to add

- Cleanup buildings a bit, any suggestions on what to add to them?
- Add textures, should be fairly easy just need to find Some
- Fix the road, its a bit wonky
- Fix some cliping issues with objects generating ontop of eachother
- Adjust the lighting system a bit more
- Smooth over the movment system.
- Maybe do some shadders if I have time.
- Make the dithering better.
- If you have any other suggestions please let me know
