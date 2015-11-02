
#pragma once

#include "fileManager.h"
#include "vertexBufferElement.h"
#include "Vector2.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include <string>
#include <map>
#include <vector>

namespace vg
{
	namespace graphics
	{
		/// A map containing the usage and name of vertex attributes
		typedef std::map<uint32_t, std::string> VariableNames;
		/**
			Holds shader attribute and  ids and links itself
			*/
		class Shader
		{
		public:
			/**
				Creates a shader with default or custom vertex element attribute names
				@param attributeNames map containing the usage and name of vertex attributes
				@param attributeNames map containing the usage and name of unifrom variables
				*/
			Shader(const VariableNames& attributeNames = getDefaultAttribNames(),
				const std::vector<std::string>& uniformNames = getDefaultUniformNames());

			/**
				Uninitializes and unloads everything.
				*/
			~Shader();

			/**
				Initialize values needed by load(). Don't call before GraphicsContext is initialized.
				*/
			void initialize();

			/**
				@return true if Shader has been initialized
				*/
			bool isInitialized();

			/**
				Loads and compiles shader sources from assets
				@param fileManager reference to an initialized FileManager
				@param vertexPath path to glsl file containing vertex shader source
				@param fragmentPath path to glsl file containing fragment shader source
				@return was shader source loading succesful
				*/
			bool load(core::FileManager& fileManager,
				const std::string& vertexPath = getDefaultVertexPath(),
				const std::string& fragmentPath = getDefaultFragmentPath());

			/**
				Returns current shader program id
				@return shader program id
				*/
			unsigned int getProgramId();

			/**
				calls glUseProgram
				*/
			void useProgram();

			/**
				calls glUseProgram(0u)
				*/
			void unUseProgram();

			/**
				@return map of vertex element ids and names
				*/
			const VariableNames& getVertexElementNames();

			/**
				@return map of uniform variable names and usages
				*/
			const std::vector<std::string>& getUniformNames();

			/**
				Set uniform matrix value
				*/
			void setUniform(std::string name, glm::mat3 value);

			/**
				Set uniform matrix value
				*/
			void setUniform(std::string name, glm::mat4 value);

			/**
				Set uniform float value
				*/
			void setUniform(std::string name, float value);

            /**
                Set value to shader uniform boolean
                */
            void setUniform(std::string name, bool value);

			/**
				Set uniform vector values
				*/
			void setUniform(std::string name, float x, float y);
			
			/**
				Set uniform vector values
				*/
			void setUniform(std::string name, float x, float y ,float z);
			
			/**
				Set uniform vector values
				*/
			void setUniform(std::string name, float x, float y, float z, float w);

		private:
			static const std::string getDefaultVertexPath();
			static const std::string getDefaultFragmentPath();
			/**
				@return default map of vertex element usages and names
				*/
			static VariableNames getDefaultAttribNames();

			/**
				@return Vector of default uniform variable names
				*/
			static std::vector<std::string> getDefaultUniformNames();

			/**
				Compiles shader source code
				@return GL_TRUE if compile was succesful
				*/
			int compileShaderSource(unsigned int id, const std::string& source);

			/**
				Prints shader ShaderInfoLog to debug log
				*/
			void printErrorLog(unsigned int shader);

			unsigned int mVertexId;								///< Vertex shader id used for linking
			unsigned int mFragmentId;								///< Fragment shader id used for linking
			unsigned int mProgramId;								///< Shader program id used for linking
			VariableNames mVertexElementNames;				///< Map of vertex element ids and names
			std::vector<std::string> mUniformNames;						///< list of uniform variable names
			std::map<std::string, unsigned int> mUniformLocations;///< Map of shader uniform names and locations
			bool mInitialized;								///< Have shaders been initialized
		};
	}
}