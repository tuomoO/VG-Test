
#include "component.h"

using namespace vg;
Component::Component()
{
	mGameObject = nullptr;
}
void Component::setGameObject(GameObject *gameObject)
{
	mGameObject = gameObject;
}
GameObject *Component::getGameObject()
{
	return mGameObject;
}