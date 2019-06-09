#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
}


VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const {

	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const {
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {

	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		GLCall(glEnableVertexAttribArray(i));
		// we are defining the buffer type attribute
		// attribute is like positions, texture cordinates, 
		// a buffer can hold many attributes side by side, for example
		// [#pos0,#tex0,#normal0, ...., #posN, #texN,#normalN]
		GLCall(glVertexAttribPointer(
			i, //what index is this attribute at [position, attribute]
			element.count, // count = how many items this attribute is made of basically,
			element.type, // type of valuess
			element.nomalized, // nomralized
			layout.GetStride(), // span of each *vertex*,
			(const void*)offset //offset of **attribute**, represented as pointer
		));

		offset += element.count * VertexBufferElement::GetSizeofType(element.type);

	}


}
