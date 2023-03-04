#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "IndexBuffer.h"
#include <iostream>

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
: m_Count(count){ 
    /*Generate the buffer*/
    glGenBuffers(1, &m_RendererID);

    /* Tells that this buffer is supposed to be drawn */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

    /* Pass data to buffer*/
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const{
    /* Zero puts the buffer to a zero position and essentially unbinds it*/
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
