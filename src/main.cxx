#include <print>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "graphics/ShaderProgram.hxx"
#include "graphics/VertexArray.hxx"
#include "scenes/SceneManager.hxx"
#include "init.hxx"
#include "main.hxx"

int main() {
    const auto window = getGlfwWindow();

    SceneManager::loadScene(SceneName::Triangle);

    double previous_time_since_start = 0;

    while (!glfwWindowShouldClose(window)) {
        const double time_since_start = glfwGetTime();
        float delta_time = time_since_start - previous_time_since_start;
        previous_time_since_start = time_since_start;

        SceneManager::updateScene(delta_time);
        SceneManager::drawScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deinit();
}
