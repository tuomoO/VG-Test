
#pragma once

#include "buffer.h"
#include "vertexBufferElement.h"
#include <vector>

namespace vg
{
	namespace graphics
	{
		/**
			Default constructor that uses default type format
			*/
		class VertexBuffer :
			public Buffer<float>
		{
		public:
			/**
				Default constructor that uses default type format
				*/
			VertexBuffer();

			/**
				Constructor that uses default format
				@param raw data in vector that you want to draw
				*/
			VertexBuffer(const std::vector<float>& data);

			/**
				Constructor used if user wants custom type format
				@param raw float data in vector
				@param custom type format
				*/
			VertexBuffer(const std::vector<float>& data, const std::vector<VertexElement>& format);

			/**
				Constructor that takes in
				@param custom type format
				*/
			VertexBuffer(const std::vector<VertexElement>& format);

			/**
				Binds raw vertex data into buffer using format and stride
				*/
			void bind() override;
		private:
			const uint32_t mStride; ///< Stride used in bind. Calculated in constructor from format
			const std::vector<VertexElement> mFormat;///< Vector that has format currently in use
			static const std::vector<VertexElement>& gDefaultFormat; ///< Default vertexElement format

			//const size_t mFormatSize; ///< Size of current formats
		};
	}
}
