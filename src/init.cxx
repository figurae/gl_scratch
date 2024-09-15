#include <print>
#include <cstdlib>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "init.hxx"

GLFWwindow* getGlfwWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwSetErrorCallback(glfwErrorCallback);

    GLFWwindow* window =
        glfwCreateWindow(800, 600, "OpenGlScratch", NULL, NULL);
    if (window == NULL) {
        std::print("Failed to create GLFW window\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::print("Failed to initialize OpenGl context\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    std::print("Loaded OpenGl {0}.{1}\n", GLAD_VERSION_MAJOR(version),
               GLAD_VERSION_MINOR(version));

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    return window;
}

void deinit() {
    std::print("Shutting down gracefully...\n");
    glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int _scancode, int action,
                 int _mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        GLint polygon_mode[2];
        glad_glGetIntegerv(GL_POLYGON_MODE, polygon_mode);

        if (polygon_mode[0] == GL_FILL) {
            glad_glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glad_glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

void framebufferSizeCallback(GLFWwindow* window, GLsizei width,
                             GLsizei height) {
    glad_glViewport(0, 0, width, height);
}

void glfwErrorCallback(int error_no, const char* error_description) {
    std::print("GLFW error No. {0}: {1}\n", error_no, error_description);
    std::exit(EXIT_FAILURE);
}
