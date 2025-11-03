#ifndef GLFW_APP_H
#define GLFW_APP_H

#ifdef _WIN32
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#elif __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include <string>

class GLFWApp {
public:
    GLFWApp(int width = 800, int height = 600, const std::string& title = "Eerie City");
    ~GLFWApp();
    
    bool initialize();
    void run();
    void cleanup();
    
private:
    // Window properties
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    std::string windowTitle;
    
    // Timing
    float deltaTime;
    float lastFrame;
    
    // Camera
    float cameraYaw;
    float cameraPitch;
    float cameraX, cameraY, cameraZ;
    
    // OpenGL objects
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    
    // Framebuffer for post-processing
    GLuint framebuffer;
    GLuint textureColorBuffer;
    GLuint rbo;
    
    // Initialization methods
    bool initGLFW();
    bool initGLEW();
    bool initOpenGL();
    void setupFramebuffer();
    void setupGeometry();
    
    // Main loop methods
    void processInput();
    void update(float dt);
    void render();
    void renderPostProcess();
    
    // Callbacks
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif // GLFW_APP_H