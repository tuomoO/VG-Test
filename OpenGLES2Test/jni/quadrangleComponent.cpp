
#include "quadrangleComponent.h"
//#include <engine/assets/assetManager.h>
#include "texture.h"
#include "fileManager.h"
//#include <engine/game/game.h>

#include "glm/vec2.hpp"

using namespace vg;
using namespace vg::core;
using namespace std;

QuadrangleComponent::QuadrangleComponent() :RenderComponent(getDefaultVertices(), getDefaultIndices())
{
	mTexture = nullptr;
}

QuadrangleComponent::QuadrangleComponent(string textureName) :RenderComponent(getDefaultVertices(), getDefaultIndices())
{
	/*
	core::AssetManager* am = Game::getInstance()->getAssetManager();
	if ((am->get<vg::graphics::Texture>(textureName)) == nullptr)
		am->load<vg::graphics::Texture>(textureName);
	mTexture = am->get<vg::graphics::Texture>(textureName);
	*/
}


QuadrangleComponent::~QuadrangleComponent()
{
}

std::vector<float> QuadrangleComponent::getDefaultVertices()
{
	std::vector<float> defaultVertices = std::vector < float >
    {
        // Position Vec2
        // Color Vec4
        // TexCoord Vec2

        //left up
        0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f,

        //left down
        0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f,

        //right down
        1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f,

        //right up
        1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f
    };
	return defaultVertices;
}

std::vector<unsigned short> QuadrangleComponent::getDefaultIndices()
{
	std::vector<unsigned short> defaultIndices = std::vector <unsigned short>
	{
		0u, 1u, 2u,
		0u, 2u, 3u
	};
	return defaultIndices;
};
