#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#ifdef _WIN32
    #include <GL/glew.h>
#elif __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif

#include <string>

namespace ShaderUtils {
    // Load shader source from file
    std::string loadShaderSource(const std::string& filepath);
    
    // Compile a shader from source
    GLuint compileShader(GLenum type, const std::string& source);
    
    // Create shader program from vertex and fragment shader files
    GLuint createProgram(const std::string& vertexPath, const std::string& fragmentPath);
    
    // Check shader compilation errors
    bool checkCompileErrors(GLuint shader, const std::string& type);
    
    // Check program linking errors
    bool checkLinkErrors(GLuint program);
}

#endif // SHADER_UTILS_H