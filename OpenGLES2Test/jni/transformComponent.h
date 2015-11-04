
#pragma once

#include "component.h"
#include "Vector2.h"

namespace vg
{

	/**
	Component for managing gameObject by moving, rotating or scaling
	*/
	class TransformComponent :
		public Component
	{
	public:

		TransformComponent();
		TransformComponent(const TransformComponent &transform);
        /**
        @param textureFileName path to texture
        @param position position of upper left corner in pixels
        @param size width and lenght of the sprite in pixels
        @param rotation angle of rotation clockwise
        @param layer higher layers are drawn over lower ones
        @param origin offset of origin in pixels from upper left corner
		@param useCamera true if the position is affected by camera
        */
		TransformComponent(vg::Vector2<float> position, vg::Vector2<float> size,
			float rotation, unsigned int layer = getDefaultLayer(),
			vg::Vector2<float> origin = vg::Vector2<float>(0, 0), bool useCamera = true);
		
        ~TransformComponent();

        /**
        @return local position (only taking into account this component´s position not parent´s position)
        */
        vg::Vector2<float> getLocalPosition();

		/**
		@return world position (adds parent gameobject´s position to this components local position)
		*/
		vg::Vector2<float> getWorldPosition();
        /**
        Set position value to be used on draw calls.
        */
        void setPosition(const vg::Vector2<float> position);

        /**
        Add to the position value to be used on draw calls.
        */
        void move(vg::Vector2<float> change);

        /**
		@return size in pixels
        */
        vg::Vector2<float> getSize();


        /**
        Set position value to be used on draw calls.
        */
        void setSize(const vg::Vector2<float> size);

        /**
		@return rotation in degrees
        */
        float getLocalRotation();
		/**
		stuff
		*/
		float getWorldRotation();
        /**
        Set rotation value to be used on draw calls.
        */
        void setRotation(float rotation);

        /**
        Add to the rotation value
        */
        void rotate(float rotatation);

        /**
        @return layer value clamped to range of 0-1.
        */
        float getLayer();

        /**
        @param layer value ranging from 0 to 10000.
        */
        void setLayer(unsigned int layer);

        /**
        @return origin offset
        */
        vg::Vector2<float> getOrigin();

        /**
        Set origin offset.
        */
        void setOrigin(const vg::Vector2<float> origin);

		/**
		@param value true if the position is affected by camera
		*/
		void setUsingCamera(bool value);

		/**
		@return true if the position is affected by camera
		*/
		bool getUsingCamera();

	private:
		/**
		@return Returns the default layer
		*/
		static unsigned int getDefaultLayer();	

		static unsigned int mCurrentLayer;		///< Points at the current layer
        vg::Vector2<float> mPosition;				///< Position of top left corner in pixels.
		vg::Vector2<float> mOrigin;				///< origin offset from upper left corner in pixels
		vg::Vector2<float> mSize;					///< Sprites witdth and length in pixels.
        float mRotation;						///< Rotation of sprite in angles.
        unsigned int mLayer;					///< Layer where the sprite will be drawn.
		bool mUsingCamera;						///< Is the position affected by camera?
	};

}