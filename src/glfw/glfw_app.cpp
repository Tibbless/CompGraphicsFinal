#include "glfw/glfw_app.h"
#include "common/shader_utils.h"
#include "common/math_utils.h"
#include <iostream>

// Camera settings
static float lastX = 400, lastY = 300;
static bool firstMouse = true;

GLFWApp::GLFWApp(int width, int height, const std::string& title)
    : window(nullptr), windowWidth(width), windowHeight(height), windowTitle(title),
      deltaTime(0.0f), lastFrame(0.0f),
      cameraYaw(-90.0f), cameraPitch(0.0f),
      cameraX(0.0f), cameraY(2.0f), cameraZ(5.0f),
      shaderProgram(0), VAO(0), VBO(0), EBO(0),
      framebuffer(0), textureColorBuffer(0), rbo(0) {
}

GLFWApp::~GLFWApp() {
    cleanup();
}

bool GLFWApp::initialize() {
    if (!initGLFW()) return false;
    if (!initGLEW()) return false;
    if (!initOpenGL()) return false;
    
    setupFramebuffer();
    setupGeometry();
    
    std::cout << "Initialization complete!" << std::endl;
    std::cout << "Controls: WASD to move, Mouse to look, ESC to exit" << std::endl;
    
    return true;
}

bool GLFWApp::initGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetWindowUserPointer(window, this);
    
    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    return true;
}

bool GLFWApp::initGLEW() {
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }
#endif
    return true;
}

bool GLFWApp::initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, windowWidth, windowHeight);
    
    // Load shaders
    shaderProgram = ShaderUtils::createProgram(
        "shaders/basic.vert",
        "shaders/basic.frag"
    );
    
    if (shaderProgram == 0) {
        std::cerr << "Failed to create shader program" << std::endl;
        return false;
    }
    
    return true;
}

void GLFWApp::setupFramebuffer() {
    // Create framebuffer for low-res rendering
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    // Create texture for color buffer (low resolution for pixelation)
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 320, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
    
    // Create renderbuffer for depth and stencil
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 320, 240);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFWApp::setupGeometry() {
    // Simple cube for testing
    float vertices[] = {
        // positions          // colors
        -0.5f, -0.5f, -0.5f,  0.2f, 0.2f, 0.3f,
         0.5f, -0.5f, -0.5f,  0.3f, 0.2f, 0.3f,
         0.5f,  0.5f, -0.5f,  0.3f, 0.3f, 0.4f,
        -0.5f,  0.5f, -0.5f,  0.2f, 0.3f, 0.4f,
        -0.5f, -0.5f,  0.5f,  0.2f, 0.2f, 0.3f,
         0.5f, -0.5f,  0.5f,  0.3f, 0.2f, 0.3f,
         0.5f,  0.5f,  0.5f,  0.3f, 0.3f, 0.4f,
        -0.5f,  0.5f,  0.5f,  0.2f, 0.3f, 0.4f
    };
    
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        1, 5, 6, 6, 2, 1,
        5, 4, 7, 7, 6, 5,
        4, 0, 3, 3, 7, 4,
        3, 2, 6, 6, 7, 3,
        4, 5, 1, 1, 0, 4
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void GLFWApp::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = 2.5f * deltaTime;
    
    MathUtils::Vec3 forward(
        std::cos(MathUtils::radians(cameraYaw)) * std::cos(MathUtils::radians(cameraPitch)),
        std::sin(MathUtils::radians(cameraPitch)),
        std::sin(MathUtils::radians(cameraYaw)) * std::cos(MathUtils::radians(cameraPitch))
    );
    forward = forward.normalize();
    
    MathUtils::Vec3 right(forward.z, 0, -forward.x);
    right = right.normalize();
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraX += forward.x * cameraSpeed;
        cameraY += forward.y * cameraSpeed;
        cameraZ += forward.z * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraX -= forward.x * cameraSpeed;
        cameraY -= forward.y * cameraSpeed;
        cameraZ -= forward.z * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraX -= right.x * cameraSpeed;
        cameraZ -= right.z * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraX += right.x * cameraSpeed;
        cameraZ += right.z * cameraSpeed;
    }
}

void GLFWApp::update(float dt) {
    // Update logic here (animations, physics, etc.)
}

void GLFWApp::render() {
    // Render to framebuffer (low-res)
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, 320, 240);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shaderProgram);
    
    // Set up matrices
    MathUtils::Mat4 projection = MathUtils::Mat4::perspective(
        MathUtils::radians(45.0f),
        320.0f / 240.0f,
        0.1f, 100.0f
    );
    
    MathUtils::Vec3 cameraPos(cameraX, cameraY, cameraZ);
    MathUtils::Vec3 cameraFront(
        std::cos(MathUtils::radians(cameraYaw)) * std::cos(MathUtils::radians(cameraPitch)),
        std::sin(MathUtils::radians(cameraPitch)),
        std::sin(MathUtils::radians(cameraYaw)) * std::cos(MathUtils::radians(cameraPitch))
    );
    MathUtils::Mat4 view = MathUtils::Mat4::lookAt(
        cameraPos,
        cameraPos + cameraFront,
        MathUtils::Vec3(0.0f, 1.0f, 0.0f)
    );
    
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.m);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.m);
    
    // Render multiple cubes
    glBindVertexArray(VAO);
    for (int i = -5; i < 5; i++) {
        for (int j = -5; j < 5; j++) {
            MathUtils::Mat4 model = MathUtils::Mat4::translate(
                MathUtils::Vec3(i * 2.0f, 0.0f, j * 2.0f)
            );
            GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
    }
    
    // Render to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderPostProcess();
}

void GLFWApp::renderPostProcess() {
    // For now, just render the texture to a fullscreen quad
    // Later we'll add post-processing effects here
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    
    // Draw fullscreen quad (simplified for now)
    // TODO: Add proper post-processing shader
}

void GLFWApp::run() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput();
        update(deltaTime);
        render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void GLFWApp::cleanup() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
    if (framebuffer) glDeleteFramebuffers(1, &framebuffer);
    if (textureColorBuffer) glDeleteTextures(1, &textureColorBuffer);
    if (rbo) glDeleteRenderbuffers(1, &rbo);
    
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void GLFWApp::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    GLFWApp* app = static_cast<GLFWApp*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->windowWidth = width;
        app->windowHeight = height;
    }
}

void GLFWApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Additional key handling can go here
}

void GLFWApp::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    GLFWApp* app = static_cast<GLFWApp*>(glfwGetWindowUserPointer(window));
    if (!app) return;
    
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    app->cameraYaw += xoffset;
    app->cameraPitch += yoffset;
    
    if (app->cameraPitch > 89.0f) app->cameraPitch = 89.0f;
    if (app->cameraPitch < -89.0f) app->cameraPitch = -89.0f;
}