
#pragma once

#include "fileManager.h"
#include <stdint.h>
#include <string>

class Texture
{
public:
	/**
		Constructor
		@param path Filepath for the texture, must be .png
		*/
	Texture();

	~Texture() = default;

	/**
		<description>
		@param fileManager <description>
		@return <description>
		*/
	bool load(FileManager* fm, const std::string& path);

	/**
		Deletes the loaded texture
		@return <description>
		*/
	bool unload();

	/**
		Binds the loaded texture
		@return Returns whether the texture is loaded or not
		*/
	void bind() const;

	/**
		Unbinds the loaded texture
		*/
	void unbind() const;

	/**
		Enables smoothing for the texture
		*/
	void setSmoothing(bool enable) const;

	/**
		@return Returns texture id
		*/
	uint32_t getId() const;

	/**
		@return Returns texture width
		*/
	uint32_t getWidth() const;

	/**
		@return Returns texture height
		*/
	uint32_t getHeight() const;
private:
	void init();

	bool mIsLoaded;
	uint32_t mWidth = 0;	///< Texture width
	uint32_t mHeight = 0;	///< Texture height
	uint32_t mId = 0;		///< Texture id
};