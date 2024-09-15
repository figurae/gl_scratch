#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

GLFWwindow* getGlfwWindow();
void deinit();
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods);
void framebufferSizeCallback(GLFWwindow* window, GLsizei width, GLsizei height);
void glfwErrorCallback(int error_no, const char* error_description);
