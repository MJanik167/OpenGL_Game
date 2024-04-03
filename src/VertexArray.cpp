#include "headers/VertexArray.h"

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
  glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
  bind();
  vb.bind();
  const auto &elements = layout.getElements();
  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto &element = elements[i];
    unsigned int offset = 0;
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void *)offset);
    offset += element.count * VertexBufferElement::getSizeOfType(element.type);
  }
}

void VertexArray::bind() const
{
  glBindVertexArray(m_RendererID);
}

void VertexArray::unbind() const
{
  glBindVertexArray(0);
}