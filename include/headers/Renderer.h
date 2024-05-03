#ifndef RENDERER_H
#define RENDERER_H
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <iostream>

#pragma once

#define ASSERT(x) \
    if (!(x))     \
        __debugbreak();

#define GLCall(x)    \
    glClearErrors(); \
    x;               \
    ASSERT(glLogCall(#x, __FILE__, __LINE__))

static void glClearErrors()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

static bool glLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "):" << function << " " << file << ": " << line << std::endl;
        return false;
    }
    return true;
}

class Renderer
{
public:
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);
    void clear() const;

private:
};

#endif