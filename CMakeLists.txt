cmake_minimum_required(VERSION 3.15)
project(EerieCity VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Print build information
message(STATUS "Building Eerie City")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# Find required packages
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found! Please install OpenGL development libraries.")
endif()

find_package(glfw3 REQUIRED)
if(NOT glfw3_FOUND)
    message(FATAL_ERROR "GLFW3 not found! Please install GLFW3. See INSTALL.md for instructions.")
endif()

# GLEW is not needed on macOS
if(NOT APPLE)
    find_package(GLEW REQUIRED)
    if(NOT GLEW_FOUND)
        message(FATAL_ERROR "GLEW not found! Please install GLEW. See INSTALL.md for instructions.")
    endif()
endif()

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIRS}
)

if(NOT APPLE)
    include_directories(${GLEW_INCLUDE_DIRS})
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/common/shader_utils.cpp
    src/common/math_utils.cpp
    src/glfw/glfw_app.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
if(APPLE)
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        glfw
    )
else()
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        glfw
        GLEW::GLEW
    )
endif()

# Copy shaders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Platform-specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)
endif()

# Print success message
message(STATUS "Configuration complete!")
message(STATUS "Run 'make' or 'cmake --build .' to build the project")