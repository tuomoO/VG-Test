
#include "renderComponent.h"

using namespace vg;

RenderComponent::RenderComponent(std::vector<float> vertices, std::vector<unsigned short> indices) :Component()
{
	this->vertices = vertices;
	this->indices = indices;
	mTexture = nullptr;
}

vg::graphics::Texture *RenderComponent::getTexture()
{
	return mTexture;
}

void RenderComponent::setTexCoords(glm::vec2 texCoords[4])
{
	for (int i = 0; i < 4; i++)
	{
		vertices[i * 8 + 6] = texCoords[i].x;
		vertices[i * 8 + 7] = texCoords[i].y;
	}
}