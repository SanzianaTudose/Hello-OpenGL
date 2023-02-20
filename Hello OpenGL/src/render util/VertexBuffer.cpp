#include "VertexBuffer.h"
#include "Renderer.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int drawMode) {
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, drawMode));

}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::AddData(unsigned int offset, unsigned int size, const void* data) {
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
