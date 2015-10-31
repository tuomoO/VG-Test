
#include "indexBuffer.h"

IndexBuffer::IndexBuffer()
	: Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
}

IndexBuffer::IndexBuffer(const std::vector<unsigned short>& data)
	: Buffer(GL_ELEMENT_ARRAY_BUFFER, data, GL_DYNAMIC_DRAW)
{
}
