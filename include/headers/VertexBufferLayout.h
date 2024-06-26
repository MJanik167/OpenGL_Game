#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_BYTE:
            return 1;
        }
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout()
        : m_Stride(0){};

    inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; };
    inline unsigned int getStride() const { return m_Stride; };

    void push_float(unsigned int count)
    {
        m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
        m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
    }

    void push_uint(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
    }

    void push_uchar(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
    }
};