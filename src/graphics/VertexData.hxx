#pragma once

#include <bit>
#include <concepts>
#include <cstddef>
#include <span>
#include <vector>
#include <glad/gl.h>

template <typename T>
GLenum getGlEnum();

enum VertexAttributeType {
    Position,
    Color,
};

class VertexData {
public:
    std::span<const std::byte> getByteData() const;
    GLenum getDataType() const;
    GLsizeiptr getByteDataSize() const;

protected:
    template <typename T>
    explicit VertexData(const std::vector<T>& data);

    std::vector<std::byte> m_data;
    GLenum m_data_type;
    GLsizei m_data_size;
};

class VertexIndices : public VertexData {
public:
    template <typename T>
    requires std::same_as<T, GLuint> || std::same_as<T, GLushort> ||
             std::same_as<T, GLubyte>
    explicit VertexIndices(const std::vector<T>& data);

    GLsizei getIndexCount() const;
};

class VertexAttributes : public VertexData {
public:
    template <typename T>
    // TODO: GLhalf and GLfixed support
    // (also, clang-format seems to mess requires in the definition)
    requires std::same_as<T, GLfloat> || std::same_as<T, GLdouble> ||
             std::same_as<T, GLuint> || std::same_as<T, GLint> ||
             std::same_as<T, GLushort> || std::same_as<T, GLshort> ||
             std::same_as<T, GLubyte> || std::same_as<T, GLbyte>
    explicit VertexAttributes(VertexAttributeType attribute_type,
                              const std::vector<T>& data);

    GLint getComponentsPerVertex() const;
    GLsizei getAttributeSize() const;
    GLsizei getVertexCount() const;

private:
    GLint m_components_per_vertex;
    GLsizei m_attribute_size;

    std::size_t getComponentsPerType(VertexAttributeType attribute_type) const;
};

// template definitions go below

template <typename T>
VertexData::VertexData(const std::vector<T>& data)
    : m_data_size(data.size()), m_data_type(getGlEnum<T>()) {
    const std::size_t byte_size = sizeof(T) * data.size();
    const auto* bytes = std::bit_cast<std::byte*>(data.data());

    m_data.reserve(byte_size);
    m_data.insert(m_data.begin(), bytes, bytes + byte_size);
}

template <typename T>
requires std::same_as<T, GLuint> || std::same_as<T, GLushort> ||
         std::same_as<T, GLubyte>
VertexIndices::VertexIndices(const std::vector<T>& data)
    : VertexData(std::move(data)) {}

template <typename T>
requires std::same_as<T, GLfloat> || std::same_as<T, GLdouble> ||
             std::same_as<T, GLuint> || std::same_as<T, GLint> ||
             std::same_as<T, GLushort> || std::same_as<T, GLshort> ||
             std::same_as<T, GLubyte> || std::same_as<T, GLbyte>
VertexAttributes::VertexAttributes(VertexAttributeType attribute_type,
                                   const std::vector<T>& data)
    : VertexData(std::move(data)),
      m_components_per_vertex(getComponentsPerType(attribute_type)),
      m_attribute_size(getComponentsPerType(attribute_type) * sizeof(T)) {}
