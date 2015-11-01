#include <string>

#include "shader.h"
#include "log.h"
#include "opengl.h"

const std::string FOLDER = "shaders/"; ///< subfolder for shader sources
using namespace std;
using namespace glm;
using namespace gl;

Shader::Shader(const VariableNames& attributeNames, const std::vector<std::string>& uniformNames)
	: mVertexElementNames(attributeNames), mUniformNames(uniformNames)
{
	LOG("shader constructor");
    mInitialized = false;
}

Shader::~Shader()
{
	gl::useProgram(0u);
}

void Shader::initialize()
{
    mProgramId = createProgram();
    mVertexId = createShader(GL_VERTEX_SHADER);
	mFragmentId = createShader(GL_FRAGMENT_SHADER);
	if (!mVertexElementNames.empty())
	{
		LOG("mVertexElementNames size = %d", mVertexElementNames.size());
		for (auto& pair : mVertexElementNames)
		{
			LOG("vertex element %s", pair.second.c_str());
			bindAttribLocation(mProgramId, pair.first, pair.second.c_str());
		}
	}
    mInitialized = true;
}

bool Shader::isInitialized()
{
    return mInitialized;
}

bool Shader::load(FileManager& fileManager, const std::string& vertexPath, const std::string& fragmentPath)
{
	LOG("shader initialize begin");
    if (!mInitialized)
        initialize();
	LOG("shader initialize end");

    // compile shaders
    std::string buffer;
	LOG("shader read file begin");
    fileManager.readAsset(FOLDER + vertexPath, buffer);
	if (compileShaderSource(mVertexId, buffer) != GL_TRUE)
    {
        Log("vgengine", "Vertex shader compile error!", "");
        printErrorLog(mVertexId);
        return false;
    }

    fileManager.readAsset(FOLDER + fragmentPath, buffer);
    if (compileShaderSource(mFragmentId, buffer) != GL_TRUE)
    {
        Log("vgengine", "Fragment shader compile error!", "");
        printErrorLog(mFragmentId);
        return false;
    }
	LOG("shader read file end");

    // link program
    gl::attachShader(mProgramId, mVertexId);
	gl::attachShader(mProgramId, mFragmentId);
    gl::linkProgram(mProgramId);
	LOG("shader program linked");

	if (gl::linkStatus(mProgramId) != GL_TRUE)
    {
        Log("vgengine", "Shader program link error!", "");
        return false;
    }
    
	//uniforms
	LOG("uniforms begin");
    gl::useProgram(mProgramId);
	
	if (!mVertexElementNames.empty())
	{
		LOG("uniform names size = %d", mUniformNames.size());
		for (int i = 0; i < mUniformNames.size(); i++)
			mUniformLocations.insert(make_pair<string&, unsigned int>(mUniformNames[i], 0));
		for (auto& pair : mUniformLocations)
		{
			unsigned int location = gl::getUniformLocation(mProgramId, pair.first);
			if (location < 0)
			{
				Log("vgengine", "Shader uniform %s not found!", pair.first.c_str());
			}
			pair.second = location;
		}
	}
    gl::useProgram(0u);
	LOG("uniforms end");

    return true;
}

unsigned int Shader::getProgramId()
{
    return mProgramId;
}

void Shader::useProgram()
{
	gl::useProgram(mProgramId);
}

void Shader::unUseProgram()
{
	gl::useProgram(0u);
}

const VariableNames& Shader::getVertexElementNames()
{
    return mVertexElementNames;
}

const std::vector<std::string>& Shader::getUniformNames()
{
	return mUniformNames;
}

VariableNames Shader::getDefaultAttribNames()
{
	VariableNames result;
    result[Position] = "attrPosition";
    result[Color] = "attrColor";
    result[TexCoord] = "attrTexCoord";
    return result;
}

vector<string> Shader::getDefaultUniformNames()
{
	return vector<string>
	{
		"unifProjection",
		"unifModel",
		"unifLayer",
		"unifNoTexture",
		"unifFontTexture"
	};
};

int Shader::compileShaderSource(unsigned int id, const std::string& source)
{
    int result = GL_FALSE;
    const char* temp = source.c_str();
    shaderSource(id, 1, &temp, NULL);
    compileShader(id);
    getShaderiv(id, GL_COMPILE_STATUS, &result);
    return result;
}

void Shader::printErrorLog(unsigned int shader)
{
    int bufferLenght;
    getShaderiv(shader, GL_INFO_LOG_LENGTH, &bufferLenght);
    vector<char> buffer(bufferLenght);
    getShaderInfoLog(shader, buffer.size(), nullptr, buffer.data());
    Log("vgengine", "%s", buffer.data());
}

void Shader::setUniform(string name, glm::mat3 value)
{
	gl::setUniform(mUniformLocations[name], value);
}

void Shader::setUniform(string name, mat4 value)
{
	gl::setUniform(mUniformLocations[name], value);
}

void Shader::setUniform(string name, float value)
{
	gl::setUniform(mUniformLocations[name], &value);
}

void Shader::setUniform(string name, bool value)
{
    float shaderValue;
    value ? shaderValue = 1.0f : shaderValue = 0.0f;
	gl::setUniform(mUniformLocations[name], &shaderValue);
}

void Shader::setUniform(string name, float x ,float y)
{
	glm::vec2 shaderValue(x, y);
	gl::setUniform(mUniformLocations[name], shaderValue);
}

void Shader::setUniform(std::string name, float x, float y, float z)
{
	glm::vec3 shaderValue(x, y, z);
	gl::setUniform(mUniformLocations[name], shaderValue);
}

void Shader::setUniform(std::string name, float x, float y, float z, float w)
{
	glm::vec4 shaderValue(x, y, z, w);
	gl::setUniform(mUniformLocations[name], shaderValue);
}
const std::string Shader::getDefaultVertexPath()
{
	return "android_vertex.glsl";
}
const std::string Shader::getDefaultFragmentPath()
{
	return "android_fragment.glsl";
}