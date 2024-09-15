#include <cstddef>
#include <span>
#include <utility>
#include <vector>
#include <glad/gl.h>
#include "VertexData.hxx"

GLint VertexAttributes::getComponentsPerVertex() const {
    return m_components_per_vertex;
}
GLsizei VertexAttributes::getAttributeSize() const { return m_attribute_size; }
GLsizei VertexAttributes::getVertexCount() const {
    return m_data_size / m_components_per_vertex;
}

// NOTE: if this doesn't get any larger, maybe just cast enums to
// values?
std::size_t VertexAttributes::getComponentsPerType(
    VertexAttributeType attribute_type) const {
    switch (attribute_type) {
    case Position:
        return 3;
    case Color:
        return 4;
    }
}

GLsizei VertexIndices::getIndexCount() const { return m_data_size; }

std::span<const std::byte> VertexData::getByteData() const {
    return {m_data.data(), m_data.size()};
};
GLenum VertexData::getDataType() const { return m_data_type; }
GLsizeiptr VertexData::getByteDataSize() const { return m_data.size(); }
