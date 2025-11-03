# Eerie City - PS1 Style 3D Renderer

A low-resolution, pixelated 3D cityscape with dithering and distortion effects to create an unsettling, surreal atmosphere inspired by PS1-era horror games.

## Features (Planned)
- Low-resolution rendering (320x240 upscaled)
- Dithering effects
- Color depth reduction
- Vertex jitter / affine texture mapping wobble
- Chromatic aberration
- Procedurally generated city buildings
- Atmospheric fog with banding
- VHS-style distortion

## Dependencies

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install build-essential cmake
sudo apt-get install libglfw3-dev libglew-dev
sudo apt-get install libgl1-mesa-dev
```

### macOS
```bash
brew install cmake glfw glew
```

### Windows
- Install CMake from https://cmake.org/download/
- Install GLFW from https://www.glfw.org/download.html
- Install GLEW from http://glew.sourceforge.net/
- Or use vcpkg:
```bash
vcpkg install glfw3 glew
```

## Building

```bash
mkdir build
cd build
cmake ..
make
./EerieCity
```

## Controls
- **WASD** - Move camera
- **Mouse** - Look around
- **ESC** - Exit

## Project Structure
```
eerie-city/
├── CMakeLists.txt
├── include/
│   ├── common/          # Shared utilities
│   └── glfw/            # GLFW application
├── src/
│   ├── common/          # Shader & math utilities
│   ├── glfw/            # Main application
│   └── main.cpp
└── shaders/             # GLSL shaders
```

## Current Status
✅ Basic GLFW + OpenGL setup
✅ Camera controls (FPS-style)
✅ Low-resolution framebuffer (320x240)
✅ Basic geometry rendering
⬜ Post-processing effects (dither, color reduction)
⬜ City generation
⬜ Advanced distortion effects

## Next Steps
1. Implement post-processing shader pipeline
2. Add dithering (Bayer matrix)
3. Implement color depth reduction
4. Create city building generator
5. Add atmospheric effects (fog, lighting)
6. Implement vertex jitter for PS1 wobble effect

## Technical Notes

### Rendering Pipeline
1. Scene renders to 320x240 framebuffer
2. Post-processing shaders applied
3. Upscaled to screen with nearest-neighbor filtering

### Shader Effects
- **Dithering**: Ordered dithering using Bayer matrix
- **Color Reduction**: Quantize RGB to 5-6 bits per channel
- **Distortion**: Screen-space warping effects
- **Chromatic Aberration**: RGB channel offset

## License
Educational project - feel free to use and modify!