
#pragma once

#include "renderComponent.h"

namespace vg
{
	class QuadrangleComponent : public RenderComponent
	{
	public:
		/**
		Default rectangle render component without texture
		*/
		QuadrangleComponent();

		/**
		Rectangle render component with texture
		@param textureName texture file path
		*/
		QuadrangleComponent(std::string textureName);

		~QuadrangleComponent();

	private:
		std::vector<float> getDefaultVertices();
		std::vector<unsigned short> getDefaultIndices();
	};
}