
#include "gameObject.h"
//#include <engine/game/game.h>
#include "renderComponent.h"
#include "quadrangleComponent.h"
//#include <engine/game/triangleComponent.h>
#include "texture.h"
#include "transformComponent.h"

#include <algorithm>

using namespace vg;
using namespace vg::graphics;

GameObject::GameObject(std::string name)
	: BaseClass("gameObject"), mName(name), mMarkedForDelete(false)
{
	mParent = nullptr;
}

GameObject::GameObject(const GameObject &obj) 
	: BaseClass(obj.mName), mName(obj.mName), mMarkedForDelete(false)
{
	mParent = nullptr;
	for (auto ij = obj.mComponents.begin(); ij != obj.mComponents.end(); ij++)
	{
		
		if (dynamic_cast<RenderComponent*>(ij->second))
		{
			if (dynamic_cast<QuadrangleComponent*>(ij->second))
			{
				//Game *game = Game::getInstance();
				QuadrangleComponent *comppp = dynamic_cast<QuadrangleComponent*>(ij->second);
				std::string path = comppp->getTexture()->getPath();
				QuadrangleComponent *temp = new QuadrangleComponent(path);
				mComponents.insert(std::make_pair(&typeid(*temp), temp));
			}
			/*
			else if (typeid((TriangleComponent*)ij->second) == typeid(TriangleComponent))
			{
				TriangleComponent *temp = new TriangleComponent();
				mComponents.insert(std::make_pair(&typeid(*temp), temp));
			}
			*/
			else
			{
				Log("vgengine", "error!!!!", "");
			}
		
		}
		else if (dynamic_cast<TransformComponent*>(ij->second))
		{
			TransformComponent *t = new TransformComponent(*dynamic_cast<TransformComponent*>(ij->second));
			mComponents.insert(std::make_pair(&typeid(*ij->second),t));
		}
	}
}

void GameObject::addComponent(Component* component)
{
	component->setGameObject(this);
	mComponents.insert(std::make_pair(&typeid(*component), component));

}

void GameObject::markForDelete()
{
	mMarkedForDelete = true;
}

bool GameObject::markedForDelete()
{
	return mMarkedForDelete;
}

void GameObject::setParent(GameObject *parent)
{
	mParent = parent;
}
GameObject *GameObject::getParent()
{
	return mParent;
}