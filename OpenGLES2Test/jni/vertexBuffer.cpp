
#include "vertexBuffer.h"
#include "opengl.h"

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
	: Buffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
	, mFormat(format)
	, mStride(countStride(format))
{
}

VertexBuffer::VertexBuffer(const std::vector<float>& data)
	: VertexBuffer(data, gDefaultFormat)
{
}

VertexBuffer::VertexBuffer(const std::vector<float>& data, const std::vector<VertexElement>& format)
	: Buffer(GL_ARRAY_BUFFER, data, GL_DYNAMIC_DRAW)
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
		vertexAttribPointer(mFormat[i].mUsage, mFormat[i].mType, GL_FLOAT, GL_FALSE, mStride * sizeof(float), (void*)(offset));
		offset += mFormat[i].mType * sizeof(float);
	}
}
