
#pragma once

#include <stdint.h>

namespace vg
{
	namespace graphics
	{
		/**
		Contains usage types for vertexBuffer
		*/
		enum VertexElementUsage
		{
			Position,
			Color,
			TexCoord,

			UsageCOUNT
		};

		/**
		Contains types for verteBuffer
		*/
		enum VertexElementType
		{
			Float = 1,
			Vec2 = 2,
			Vec3 = 3,
			Vec4 = 4,
			Mat2 = 2 * 2,
			Mat3 = 3 * 3,
			Mat4 = 4 * 4,

			TypeCOUNT
		};

		/**
		<description>
		*/
		struct VertexElement
		{
			uint32_t mUsage;
			uint32_t mType;
		};
	}
}
