#include <cstdlib>
#include <print>
#include <vector>
#include <glad/gl.h>
#include "VertexArray.hxx"
#include "VertexData.hxx"

VertexArray::VertexArray(
    const std::vector<const VertexAttributes>& attributes) {
    if (attributes.empty()) {
        std::print("Empty attribute object given to vao {0}\n", m_vao);
        std::exit(EXIT_FAILURE);
    }

    GLsizeiptr vbo_size{attributes[0].getByteDataSize()};
    GLsizei vbo_stride{attributes[0].getAttributeSize()};

    m_vertex_count = attributes[0].getVertexCount();

    for (std::size_t i = 1; i < attributes.size(); ++i) {
        const auto current_vertex_count = attributes[i].getVertexCount();

        if (current_vertex_count != m_vertex_count) {
            // TODO: add a way to identify attributes
            std::print(
                "Vertex count in vao {0} differs between vectors {1} ({2}) "
                "and {3} ({4})\n",
                m_vao, 0, m_vertex_count, i, current_vertex_count);
            std::exit(EXIT_FAILURE);
        }

        vbo_size += attributes[i].getByteDataSize();
        vbo_stride += attributes[i].getAttributeSize();
    }

    if (vbo_size == 0) {
        std::print("No vertex attributes given to vao {0}\n", m_vao);
        std::exit(EXIT_FAILURE);
    }

    glad_glGenVertexArrays(1, &m_vao);
    glad_glGenBuffers(1, &m_vbo);

    glad_glBindVertexArray(m_vao);

    std::vector<std::byte> interleaved_vbo;
    interleaved_vbo.reserve(vbo_size);
    const auto attribute_count = attributes.size();

    for (std::size_t i = 0; i < m_vertex_count; ++i) {
        for (std::size_t j = 0; j < attribute_count; ++j) {
            const auto attribute_size = attributes[j].getAttributeSize();
            const auto current_attribute_pos = i * attribute_size;
            const auto current_subspan = attributes[j].getByteData().subspan(
                current_attribute_pos, attribute_size);

            interleaved_vbo.insert(interleaved_vbo.end(),
                                   current_subspan.begin(),
                                   current_subspan.end());
        }
    }

    glad_glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glad_glBufferData(GL_ARRAY_BUFFER, vbo_size, interleaved_vbo.data(),
                      GL_STATIC_DRAW);

    GLsizei offset{0};
    for (GLuint i = 0; i < attribute_count; ++i) {
        glad_glVertexAttribPointer(i, attributes[i].getComponentsPerVertex(),
                                   attributes[i].getDataType(), GL_FALSE,
                                   vbo_stride, reinterpret_cast<void*>(offset));
        glad_glEnableVertexAttribArray(i);

        offset += attributes[i].getAttributeSize();
    }

    m_gl_error = glad_glGetError();
    if (m_gl_error != GL_NO_ERROR) {
        std::print("glGetError when setting vbo in vao {0}: {1}\n", m_vao,
                   m_gl_error);
        std::exit(EXIT_FAILURE);
    }
}

VertexArray::VertexArray(const std::vector<const VertexAttributes>& attributes,
                         const std::vector<const VertexIndices>& indices)
    : VertexArray(attributes) {
    if (indices.empty()) {
        std::print("Empty indices object given to vao {0}\n", m_vao);
        std::exit(EXIT_FAILURE);
    }

    m_index_data_type = indices[0].getDataType();

    GLsizeiptr combined_ebo_size{0};
    for (std::size_t i = 0; i < indices.size(); ++i) {
        const auto current_size = indices[i].getByteDataSize();

        if (current_size == 0) {
            std::print("No indices given in vector {0} to vao {1}\n", i, m_vao);
            std::exit(EXIT_FAILURE);
        }

        if (indices[i].getDataType() != m_index_data_type) {
            std::print("In indices given to vao {0}, index data type differs "
                       "between vectors {1} and {2}\n",
                       m_vao, 0, i);
            std::exit(EXIT_FAILURE);
        }

        combined_ebo_size += current_size;

        const auto current_offset = m_index_offsets[i] + current_size;
        if (i < indices.size() - 1) {
            m_index_offsets.push_back(current_offset);
        }
        m_index_counts.push_back(indices[i].getIndexCount());
    }

    glad_glGenBuffers(1, &m_ebo);

    std::vector<std::byte> combined_ebo;
    combined_ebo.reserve(combined_ebo_size);

    for (const auto& indices_set : indices) {
        combined_ebo.insert(combined_ebo.end(),
                            indices_set.getByteData().begin(),
                            indices_set.getByteData().end());
    }

    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, combined_ebo_size,
                      combined_ebo.data(), GL_STATIC_DRAW);

    m_gl_error = glad_glGetError();
    if (m_gl_error != GL_NO_ERROR) {
        std::print("glGetError when setting ebo in vao {0}: {1}\n", m_vao,
                   m_gl_error);
        std::exit(EXIT_FAILURE);
    }
}

VertexArray::VertexArray(const std::vector<const VertexAttributes>& attributes,
                         const VertexIndices& indices)
    : VertexArray(attributes, std::vector<const VertexIndices>{indices}) {}

VertexArray::VertexArray(const VertexAttributes& attributes)
    : VertexArray(std::vector<const VertexAttributes>{attributes}){};

void VertexArray::draw() {
    glad_glBindVertexArray(m_vao);

    if (m_ebo != 0) {
        for (std::size_t i = 0; i < m_index_counts.size(); ++i) {
            glad_glDrawElements(GL_TRIANGLES, m_index_counts[i],
                                m_index_data_type,
                                reinterpret_cast<void*>(m_index_offsets[i]));
        }
    } else {
        glad_glDrawArrays(GL_TRIANGLES, 0, m_vertex_count);
    }

    m_gl_error = glad_glGetError();
    if (m_gl_error != GL_NO_ERROR) {
        std::print("glGetError when drawing vao {0}: {1}\n", m_vao, m_gl_error);
        std::exit(EXIT_FAILURE);
    }
}

// FIXME: GLhalf maps to unsigned short, just like GLushort,
// another approach is needed to support them both
//
// template <> GLenum getGlEnum<GLhalf>() {
//     return GL_HALF_FLOAT;
// }

template <>
GLenum getGlEnum<GLfloat>() {
    return GL_FLOAT;
}

template <>
GLenum getGlEnum<GLdouble>() {
    return GL_DOUBLE;
}

template <>
GLenum getGlEnum<GLuint>() {
    return GL_UNSIGNED_INT;
}

template <>
GLenum getGlEnum<GLint>() {
    return GL_INT;
}

template <>
GLenum getGlEnum<GLushort>() {
    return GL_UNSIGNED_SHORT;
}

template <>
GLenum getGlEnum<GLshort>() {
    return GL_SHORT;
}

template <>
GLenum getGlEnum<GLubyte>() {
    return GL_UNSIGNED_BYTE;
}

template <>
GLenum getGlEnum<GLbyte>() {
    return GL_BYTE;
}

// FIXME: GL_FIXED doesn't exist for some reason
//
// template <>
// GLenum getGlEnum<GLfixed>() {
//     return GL_FIXED;
// }
