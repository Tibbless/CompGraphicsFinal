# Eerie City - Project Structure

## Complete File Tree
```
eerie-city/
├── CMakeLists.txt                    # Build configuration
├── README.md                         # Project documentation
├── build.sh                          # Quick build script
│
├── include/                          # Header files
│   ├── common/
│   │   ├── shader_utils.h           # Shader loading/compilation
│   │   └── math_utils.h             # Vector/matrix math
│   └── glfw/
│       └── glfw_app.h               # Main application class
│
├── src/                              # Source files
│   ├── common/
│   │   ├── shader_utils.cpp         # Shader utilities implementation
│   │   └── math_utils.cpp           # Math utilities implementation
│   ├── glfw/
│   │   └── glfw_app.cpp             # Main application implementation
│   └── main.cpp                      # Entry point
│
└── shaders/                          # GLSL shaders
    ├── basic.vert                    # Basic vertex shader
    └── basic.frag                    # Basic fragment shader
```

## What Each Component Does

### Core Application (glfw_app.cpp)
- Window creation and management
- OpenGL context initialization
- Input handling (keyboard + mouse)
- FPS camera controls
- Main render loop
- Framebuffer setup for low-res rendering

### Shader Utilities (shader_utils.cpp)
- Load shader files from disk
- Compile vertex/fragment shaders
- Link shader programs
- Error checking and reporting

### Math Utilities (math_utils.cpp)
- 3D vector operations
- 4x4 matrix transformations
- Perspective projection
- LookAt camera matrix
- Translation, rotation, scaling

### Current Rendering Pipeline
1. **Scene Rendering** → Render 3D scene to 320x240 framebuffer
2. **Post-Processing** → Apply effects (not yet implemented)
3. **Screen Display** → Upscale to window with nearest-neighbor

### Key Features Implemented
✅ GLFW window with OpenGL 3.3 Core
✅ FPS camera with mouse look and WASD movement
✅ Low-resolution framebuffer (320x240)
✅ Basic geometry rendering (test cubes)
✅ Shader loading system
✅ Math library for 3D transformations

### Ready for Next Phase
The boilerplate is complete! You can now add:
1. Post-processing shaders (dither, color reduction)
2. City building generation
3. Advanced visual effects
4. Atmospheric elements

## Build Instructions

### Quick Build
```bash
./build.sh
./build/EerieCity
```

### Manual Build
```bash
mkdir build && cd build
cmake ..
make
./EerieCity
```

## Configuration Options

### Resolution (glfw_app.cpp:115)
```cpp
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 320, 240, ...)
```
Change `320, 240` to adjust internal rendering resolution.

### Window Size (main.cpp:7)
```cpp
GLFWApp app(800, 600, "Eerie City - PS1 Style");
```
Change `800, 600` to adjust window dimensions.

### Camera Settings (glfw_app.cpp)
- Speed: Line 180 - `float cameraSpeed = 2.5f * deltaTime;`
- FOV: Line 225 - `MathUtils::radians(45.0f)`
- Sensitivity: Line 375 - `float sensitivity = 0.1f;`

## Next Development Priorities

1. **Post-Processing Shader**
   - Create `shaders/post_process.vert` and `.frag`
   - Implement fullscreen quad rendering
   - Add dithering, color reduction effects

2. **City Generator**
   - Create `include/scene/building.h`
   - Generate random building meshes
   - Place buildings in grid pattern

3. **Enhanced Effects**
   - Vertex jitter for PS1 wobble
   - Fog with color banding
   - Chromatic aberration
   - VHS distortion

Ready to start implementing effects!