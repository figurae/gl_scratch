#include <cstdlib>
#include <fstream>
#include <print>
#include <string>
#include <glad/gl.h>
#include "Shader.hxx"

ShaderProgram::ShaderProgram(const char* vertex_shader_path,
                             const char* fragment_shader_path) {
    const auto vertex_shader =
        getCompiledShader(GL_VERTEX_SHADER, vertex_shader_path);
    const auto fragment_shader =
        getCompiledShader(GL_FRAGMENT_SHADER, fragment_shader_path);

    m_shader_program = glad_glCreateProgram();

    glad_glAttachShader(m_shader_program, vertex_shader);
    glad_glAttachShader(m_shader_program, fragment_shader);

    glad_glLinkProgram(m_shader_program);

    glad_glGetProgramiv(m_shader_program, GL_LINK_STATUS, &m_status_success);
    if (!m_status_success) {
        glad_glGetProgramInfoLog(m_shader_program, 512, NULL, m_info_log);
        std::print("Shader program linking failed: {0}\n", m_info_log);
        std::exit(EXIT_FAILURE);
    }

    glad_glDeleteShader(vertex_shader);
    glad_glDeleteShader(fragment_shader);
}

void ShaderProgram::use() { glad_glUseProgram(m_shader_program); }

void ShaderProgram::setFloat(const GLchar* variable_name, const GLfloat value) {
    glad_glUniform1f(glad_glGetUniformLocation(m_shader_program, variable_name),
                     value);
}

GLuint ShaderProgram::getCompiledShader(const GLenum shader_type,
                                        const char* shader_path) {
    const auto shader_source = getShaderSourceFromFile(shader_path);
    const GLchar* shader_source_cstr = shader_source.c_str();

    GLuint shader = glad_glCreateShader(shader_type);
    glad_glShaderSource(shader, 1, &shader_source_cstr, NULL);
    glad_glCompileShader(shader);

    glad_glGetShaderiv(shader, GL_COMPILE_STATUS, &m_status_success);
    if (!m_status_success) {
        glad_glGetShaderInfoLog(shader, 512, NULL, m_info_log);
        std::print("Shader {0} compilation failed: {1}\n", shader_path,
                   m_info_log);
        std::exit(EXIT_FAILURE);
    }

    return shader;
}

std::string ShaderProgram::getShaderSourceFromFile(
    const char* file_path) const {
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::print("Failed to open {0}\n", file_path);
        std::exit(EXIT_FAILURE);
    }

    std::string result;
    std::string line;

    while (getline(file, line)) {
        result += line + '\n';
    }

    file.close();

    return result;
}
