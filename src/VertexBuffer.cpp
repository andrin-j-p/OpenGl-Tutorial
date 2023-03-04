#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(const void *data, unsigned int size){
    /*Generate the buffer*/
    glGenBuffers(1, &m_RendererID);

    /* Tells that this buffer is supposed to be drawn */
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

    /* Pass data to buffer*/
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const{
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const{
    /* Zero puts the buffer to a zero position and essentially unbinds it*/
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}
