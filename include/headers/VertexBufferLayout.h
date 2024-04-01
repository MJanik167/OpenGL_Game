#pragma once

#include <vector>
#include <glad/glad.h>

struct VertexBufferElement
{
    unsigned int count;
    unsigned int type;
    bool normalized;
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout() : m_Stride(0) {}
    ~VertexBufferLayout();

    template <typename T>
    void push(unsigned int count, bool normalized)
    {
        static_assert(false);
    };

    inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; };
    inline unsigned int getStride() const { return m_Stride; };
};

template <>
void VertexBufferLayout::push<float>(unsigned int count, bool normalized)
{
    m_Elements.push_back({GL_FLOAT, count, false});
    m_Stride += sizeof(GLfloat);
}

template <>
void VertexBufferLayout::push<unsigned int>(unsigned int count, bool normalized)
{
    m_Elements.push_back({GL_UNSIGNED_INT, count, false});
    m_Stride += sizeof(GLuint);
}

template <>
void VertexBufferLayout::push<unsigned char>(unsigned int count, bool normalized)
{
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, false});
    m_Stride += sizeof(GLbyte);
}
