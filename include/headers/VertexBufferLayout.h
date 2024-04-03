#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>

struct VertexBufferElement
{
    unsigned int count;
    unsigned int type;
    unsigned char normalized;

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
    VertexBufferLayout();
    ~VertexBufferLayout();

    // template <typename T>
    // void push(unsigned int count)
    // {
    //     static_assert(false);
    // };

    // template <>
    // void n_push::push<float>(unsigned int count)
    // {
    //     m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
    //     m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT);
    // }

    // template <>
    // void push<unsigned int>(unsigned int count)
    // {
    //     m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    //     m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    // }

    // template <>
    // void push<unsigned char>(unsigned int count)
    // {
    //     m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    //     m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    // }

    inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; };
    inline unsigned int getStride() const { return m_Stride; };

    // TODO:
    // NAPRAWIC TO GÓWNO dsfsdfsdfsdf

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
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
    // PLACEHOLDER
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
};

// template <>
// void VertexBufferLayout::push<float>(unsigned int count)
// {
//     m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
//     m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT);
// }

// template <>
// void VertexBufferLayout::push<unsigned int>(unsigned int count)
// {
//     m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
//     m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
// }

// template <>
// void VertexBufferLayout::push<unsigned char>(unsigned int count)
// {
//     m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
//     m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
// }