#pragma once

#include <string>
#include <glad/gl.h>

class ShaderProgram {
public:
    ShaderProgram(const char* vertex_shader_path,
                  const char* fragment_shader_path);
    void use();
    void setFloat(const GLchar* variable_name, const GLfloat value);

private:
    GLuint m_shader_program;
    // TODO: handle this buffer gracefully
    GLchar m_info_log[512];
    GLint m_status_success;

    GLuint getCompiledShader(const GLenum shader_type, const char* shader_path);
    std::string getShaderSourceFromFile(const char* file_path) const;
};
