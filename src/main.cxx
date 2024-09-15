#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "graphics/Shader.hxx"
#include "graphics/VertexArray.hxx"
#include "init.hxx"
#include "main.hxx"

int main() {
    const auto window = getGlfwWindow();

    const std::vector<GLfloat> rect_positions_left = {
        -0.8f, -0.5f, 0.0f, -0.6f, -0.5f, 0.0f,
        -0.6f, 0.5f,  0.0f, -0.8f, 0.5f,  0.0f,
    };
    const std::vector<GLfloat> rect_positions_right = {
        0.6f, -0.5f, 0.0f, 0.8f, -0.5f, 0.0f,
        0.8f, 0.5f,  0.0f, 0.6f, 0.5f,  0.0f,
    };

    const std::vector<GLuint> rect_indices = {0, 1, 3, 1, 2, 3};
    const std::vector<GLuint> rect_indices1 = {0, 1, 3};
    const std::vector<GLuint> rect_indices2 = {1, 2, 3};

    const std::vector<GLfloat> rect_colors = {
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };

    const std::vector<const VertexIndices> rect_indices_split = {
        VertexIndices(rect_indices1), VertexIndices(rect_indices2)};

    const auto rect_indices_combined = VertexIndices(rect_indices);

    const std::vector<const VertexAttributes> rect_attributes_left = {
        VertexAttributes(Position, rect_positions_left),
        VertexAttributes(Color, rect_colors),
    };

    const std::vector<const VertexAttributes> rect_attributes_right = {
        VertexAttributes(Position, rect_positions_right),
        VertexAttributes(Color, rect_colors),
    };

    const std::vector<GLfloat> triangle_positions = {
        -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
    };
    const std::vector<GLfloat> triangle_colors = {
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };

    const std::vector<const VertexAttributes> triangle_attributes{
        VertexAttributes(Position, triangle_positions),
        VertexAttributes(Color, triangle_colors),
    };

    VertexArray vao1(rect_attributes_left, rect_indices_split);
    VertexArray vao2(rect_attributes_right, rect_indices_combined);
    VertexArray vao3(triangle_attributes);

    ShaderProgram shader_program("shaders/vertex.glsl",
                                 "shaders/fragment.glsl");

    while (!glfwWindowShouldClose(window)) {
        glad_glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT);

        const auto time = glfwGetTime();

        shader_program.use();
        shader_program.setFloat("time", time);
        vao1.draw();
        vao2.draw();
        vao3.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deinit();
}
