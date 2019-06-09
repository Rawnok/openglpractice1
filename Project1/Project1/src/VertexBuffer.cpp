#include "VertexBuffer.h"
#include "Renderer.h"


VertexBuffer::VertexBuffer(const void * data, unsigned int size) {

	GLCall(glGenBuffers(1, &m_RendererID)); // how many buffers do we want, and pass value which acs as id
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // what kind of buffer and whats the id of the buffer

	// kind of buffer, size, data, whats kind of draw- static or dynamic
	// static means, we wont edit the buffer but draw multiple times
	// dynamic means, we will change the buffer between frames

	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
