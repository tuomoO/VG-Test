
#include "indexBuffer.h"

using namespace vg::graphics;

IndexBuffer::IndexBuffer()
	: Buffer(getGL_ELEMENT_ARRAY_BUFFER(), getGL_DYNAMIC_DRAW())
{
}

IndexBuffer::IndexBuffer(const std::vector<unsigned short>& data)
	: Buffer(getGL_ELEMENT_ARRAY_BUFFER(), data, getGL_DYNAMIC_DRAW())
{
}
