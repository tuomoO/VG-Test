#pragma once
#include "buffer.h"
namespace vg
{
	namespace graphics
	{
		/**
		Buffer that contains data of indices
		*/
		class IndexBuffer :
			public Buffer<unsigned short>
		{
		public:
			IndexBuffer();

			/**
			Constructor for the indexbuffer
			@param data Indice data for buffer
			*/
			IndexBuffer(const std::vector<unsigned short>& data);
		};
	}
}