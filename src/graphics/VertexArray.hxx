#pragma once

#include <vector>
#include <glad/gl.h>
#include "VertexData.hxx"

class VertexArray {
public:
    VertexArray(const std::vector<const VertexAttributes>& attributes);
    VertexArray(const std::vector<const VertexAttributes>& attributes,
                const std::vector<const VertexIndices>& indices);
    VertexArray(const std::vector<const VertexAttributes>& attributes,
                const VertexIndices& indices);
    VertexArray(const VertexAttributes& attributes);

    void draw();

private:
    GLuint m_vao{0};
    GLuint m_vbo{0};
    GLuint m_ebo{0};
    GLsizei m_vertex_count{0};
    std::vector<GLsizei> m_index_counts;
    std::vector<GLsizei> m_index_offsets{0};
    GLenum m_index_data_type{GL_UNSIGNED_INT};
    GLenum m_gl_error{0};
};
