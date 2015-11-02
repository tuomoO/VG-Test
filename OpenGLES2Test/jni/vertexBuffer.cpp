
#include "vertexBuffer.h"
#include "opengl.h"
//#include <GLES2/gl2.h>
using namespace vg::graphics;

const std::vector<VertexElement>& VertexBuffer::gDefaultFormat 
{
	{ Position, Vec2 },
	{ Color, Vec4 },
	{ TexCoord, Vec2 }
};

static uint32_t countStride(const std::vector<VertexElement>& format)
{
	uint32_t stride = 0;

	for (int i = 0; i < format.size(); i++)
	{
		stride += format[i].mType;
	}
    return stride;
}

VertexBuffer::VertexBuffer()
	: VertexBuffer(gDefaultFormat)
{
}

VertexBuffer::VertexBuffer(const std::vector<VertexElement>& format)
	: Buffer(getGL_ARRAY_BUFFER(), getGL_DYNAMIC_DRAW())
	, mFormat(format)
	, mStride(countStride(format))
{
}

VertexBuffer::VertexBuffer(const std::vector<float>& data)
	: VertexBuffer(data, gDefaultFormat)
{
}

VertexBuffer::VertexBuffer(const std::vector<float>& data, const std::vector<VertexElement>& format)
	: Buffer(getGL_ARRAY_BUFFER(), data, getGL_DYNAMIC_DRAW())
	, mFormat(format)
	, mStride(countStride(format))
{
}

void VertexBuffer::bind()
{
	Buffer::bind();
	uint32_t offset = 0;

	for (int i = 0; i < mFormat.size(); i++)
	{
		enableVertexAttribArray(mFormat[i].mUsage);
		vertexAttribPointer(mFormat[i].mUsage, mFormat[i].mType, getGL_FLOAT(), getGL_FALSE(), mStride * sizeof(float), (void*)(offset));
		offset += mFormat[i].mType * sizeof(float);
	}
}
