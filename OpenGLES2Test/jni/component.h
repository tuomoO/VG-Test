#pragma once
#include <utility>

namespace vg
{
	class GameObject;
	class type_info;
	/**
		Base for all components
	*/
	class Component
	{
	public:
		Component();
		virtual ~Component() = default;
		/**
		@param GameObject that this component belongs to
		Sets gameobject that this component is part of
		*/
		void setGameObject(GameObject *gameObject);
		/**
		Gets GameObject that this component is part of
		*/
		GameObject *getGameObject();
	protected:
		GameObject* mGameObject; ///< Gameobject that this component is part of
	};
}