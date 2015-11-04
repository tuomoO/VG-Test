
#pragma once

#include "baseClass.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <typeinfo>

namespace vg
{
	class Component;
	using ComponentMap = std::unordered_map<const std::type_info*, Component*>;

	/**
	Manages an Object in the game
	*/
	class GameObject: public BaseClass
	{
	public:
		/**
		@param name for object
		*/
		GameObject(std::string name);

		/**
		@param obj original object
		*/
		GameObject(const GameObject &obj);
		virtual ~GameObject() { };

		/**
		@return Returns gameObjects name
		*/
		std::string getName(){ return mName; }

		/**
		Adds a component to the gameObject
		@param component Component we want to add for the gameObject
		*/
		void addComponent(Component* component);
		
		/**
		@return Returns component from the gameObject
		*/
		template<typename T> 
		T* getComponent()
		{
			ComponentMap::const_iterator position = mComponents.find(&typeid(T));
			if (position == mComponents.end())
				return nullptr;
			else
				return static_cast<T*>(position->second);
		}

		/**
		@return Returns all components from the gameObject
		*/
		std::unordered_map<const std::type_info*, Component*> *getAllComponents()
		{
			return &mComponents;
		}

		/**
		Mark the GameObject to be deleted later.
		*/
		void markForDelete();

		/**
		@return has the GameObject been marked for delete.
		*/
		bool markedForDelete();

		/**
		@param GameObject that is parent of this GameObject
		Sets this gameobject´s parent gameobject
		*/
		void setParent(GameObject *parent);
		/**
		@return GameObject that is parent of this GameObject
		*/
		GameObject *getParent();
	private:
		
		GameObject *mParent;///< this gameobject´s parent gameobject
		
		ComponentMap mComponents;	///< ComponentMap used by the gameObject
		std::string mName;			///< Name of the gameObject
		bool mMarkedForDelete;		///< has the GameObject been marked for delete
	};
}
