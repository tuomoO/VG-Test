#pragma once
#include "component.h"
#include <vector>
#include "texture.h"
#include "glm/vec2.hpp"
using namespace std;
namespace vg
{
	/**
	Component for rendering gameObject
	*/
	class RenderComponent : public Component
	{
	public:
		/**
		Constructor
		@param vertices vector of vertices for the draw
		@param indices vector of indices for the draw
		*/
		RenderComponent(std::vector<float> vertices, std::vector<unsigned short> indices);
		~RenderComponent() = default;

		/**
		@return Returns the components vertices
		*/
		std::vector<float> *getVertices(){ return &vertices; };

		/**
		@return Returns the components indices
		*/
		std::vector<unsigned short> *getIndices(){ return &indices; };

		/**
		@return pointer to current texture
		*/
		vg::graphics::Texture *getTexture();

		void setTexCoords(glm::vec2 texCoords[4]);
		

	protected:
		vg::graphics::Texture *mTexture;
		std::vector<float> vertices;
		std::vector<unsigned short> indices;
	};
}