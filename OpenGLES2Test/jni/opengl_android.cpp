#if defined(OS_ANDROID)
#pragma once
#include <GLES2/gl2.h>
#include "opengl.h"
#include "logger.h"

using namespace vg::graphics;
void gl::checkError()
{
	unsigned int error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Log("vgengine", "OpenGL error: %i", error, "");
	}
}
void gl::enableVertexAttribArray(unsigned int index)
{
	glEnableVertexAttribArray(index);
	checkError();
}
void gl::vertexAttribPointer(uint32_t index, int32_t size, int32_t stride, void* data)
{
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, data);
	checkError();
}
void gl::vertexAttribPointer(unsigned int indx, int size, unsigned int type, unsigned char normalized, int stride, const void* ptr)
{
	glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
	checkError();
}

void gl::drawArrays(unsigned int primitiveType, int offset, int count)
{
	glDrawArrays(primitiveType, offset, count);
	checkError();
}

void gl::drawElements(unsigned int primitiveType, int count, unsigned int indexType, const void *indices)
{
	glDrawElements(primitiveType, count, indexType, indices);
	checkError();
}

void gl::useProgram(unsigned int programId)
{
	glUseProgram(programId);
	checkError();
}

void gl::genTextures(unsigned int* textureIds, int amount)
{
	glGenTextures(amount, textureIds);
	checkError();
}

void gl::bindTexture(unsigned int textureId)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	checkError();
}

void gl::activeTexture()
{
	glActiveTexture(GL_TEXTURE0);
	checkError();
}

void gl::activeTexture(unsigned int textureIndex = GL_TEXTURE0)
{
	glActiveTexture(textureIndex);
	checkError();
}

void gl::deleteTextures(int n, const unsigned int* textures)
{
	glDeleteTextures(n,textures);
	checkError();
}
void gl::texParameteri(unsigned int pname, int parameter)
{
	glTexParameteri(GL_TEXTURE_2D, pname, parameter);
	checkError();
}

void gl::texImage2D(unsigned int width, unsigned int height, const std::vector<unsigned char>& pixels)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	checkError();
}

void gl::texImage2D(unsigned int width, unsigned int height, const std::vector<unsigned char>& pixels, unsigned int format = GL_RGBA)
{
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels.data());
	checkError();
}

void gl::texImage2D(unsigned int width, unsigned int height, const unsigned char* pixels, unsigned int format = GL_RGBA)
{
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	checkError();
}

void gl::clear()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkError();
}

void gl::clearColor(float red, float green, float blue, float alpha)
{
	glClearColor(GLclampf(red), GLclampf(green), GLclampf(blue), GLclampf(alpha));
	checkError();
}

void gl::setUniform(unsigned int location, glm::mat3& value)
{
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	checkError();
}

void gl::setUniform(unsigned int location, glm::mat4& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	checkError();
}

void gl::setUniform(unsigned int location, const float* value)
{
	glUniform1fv(location, 1, value);
	checkError();
}

void gl::setUniform(unsigned int location, glm::vec2& value)
{
	glUniform2fv(location, 1, glm::value_ptr(value));
	checkError();
}

void gl::setUniform(unsigned int location, glm::vec3& value)
{
	glUniform3fv(location, 1, glm::value_ptr(value));
	checkError();
}

void gl::setUniform(unsigned int location, glm::vec4& value)
{
	glUniform4fv(location, 1, glm::value_ptr(value));
	checkError();
}

void gl::attachShader(unsigned int program, unsigned int shader)
{
	glAttachShader(program, shader);
	checkError();
}

void gl::linkProgram(unsigned int program)
{
	glLinkProgram(program);
	checkError();
}

int gl::linkStatus(unsigned int program)
{
	int result = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	checkError();
	return result;
}

unsigned int gl::getUniformLocation(unsigned int program, std::string name)
{
	unsigned int location = glGetUniformLocation(program, name.c_str());
	checkError();
	return location;
}
void gl::bindBuffer(unsigned int target, unsigned int buffer)
{
	glBindBuffer(target, buffer);
	checkError();
}
void gl::deleteBuffers(int n, const unsigned int* buffers)
{
	glDeleteBuffers(n, buffers);
	checkError();
}

void gl::bufferData(unsigned int target, signed   long  int size, const void* data, unsigned int usage)
{
	glBufferData(target, size, data, usage);
	checkError();
}
void gl::genBuffers(int n, unsigned int* buffers)
{
	glGenBuffers(n, buffers);
	checkError();
}

void gl::shaderSource(unsigned int shader, int count, const char** string, const int* length)
{
	glShaderSource(shader, count, string, length);
	checkError();
}


void gl::getShaderInfoLog(unsigned int shader, int bufsize, int* length, char* infolog)
{
	glGetShaderInfoLog(shader, bufsize, length, infolog);
	checkError();
}


void gl::getShaderiv(unsigned int shader, unsigned int pname, int* params)
{
	glGetShaderiv(shader, pname, params);
	checkError();
}


unsigned int gl::createProgram(void)
{
	unsigned int response = glCreateProgram();
	checkError();
	return response;
}


unsigned int gl::createShader(unsigned int type)
{
	unsigned int response = glCreateShader(type);
	checkError();
	return response;
}


void gl::bindAttribLocation(unsigned int program, unsigned int index, const char* name)
{
	glBindAttribLocation(program, index, name);
	checkError();
}

void gl::compileShader(unsigned int shader)
{
	glCompileShader(shader);
	checkError();
}
unsigned int gl::getGL_FLOAT()
{
	return GL_FLOAT;
}

unsigned int gl::getGL_CLAMP_TO_EDGE()
{
	return GL_CLAMP_TO_EDGE;
}

unsigned int gl::getGL_COLOR_BUFFER_BIT()
{
	return GL_COLOR_BUFFER_BIT;
}

unsigned int gl::getGL_ELEMENT_ARRAY_BUFFER()
{
	return GL_ELEMENT_ARRAY_BUFFER;
}

unsigned int gl::getGL_LINEAR()
{
	return GL_LINEAR;
}

unsigned int gl::getGL_TEXTURE_MAG_FILTER()
{
	return GL_TEXTURE_MAG_FILTER;
}

unsigned int gl::getGL_TEXTURE_MIN_FILTER()
{
	return GL_TEXTURE_MIN_FILTER;
}

unsigned int gl::getGL_TEXTURE_WRAP_S()
{
	return GL_TEXTURE_WRAP_S;
}

unsigned int gl::getGL_TEXTURE_WRAP_T()
{
	return GL_TEXTURE_WRAP_T;
}
unsigned int gl::getGL_ALPHA()
{
	return GL_ALPHA;
}

unsigned int gl::getGL_DYNAMIC_DRAW()
{
	return GL_DYNAMIC_DRAW;
}
unsigned int gl::getGL_ARRAY_BUFFER()
{
	return GL_ARRAY_BUFFER;
}

unsigned int gl::getGL_FALSE()
{
	return GL_FALSE;
}
unsigned int gl::getGL_TRUE()
{
	return GL_TRUE;
}
unsigned int gl::getGL_UNSIGNED_SHORT()
{
	return GL_UNSIGNED_SHORT;
}

unsigned int gl::getGL_TRIANGLES()
{
	return GL_TRIANGLES;
}


unsigned int gl::getGL_NEAREST()
{
	return GL_NEAREST;
}

unsigned int gl::getGL_COMPILE_STATUS()
{
	return GL_COMPILE_STATUS;
}

unsigned int gl::getGL_FRAGMENT_SHADER()
{
	return GL_FRAGMENT_SHADER;
}

unsigned int gl::getGL_VERTEX_SHADER()
{
	return GL_VERTEX_SHADER;
}

unsigned int gl::getGL_INFO_LOG_LENGTH()
{
	return GL_INFO_LOG_LENGTH;
}
unsigned int gl::getGL_DEPTH_BUFFER_BIT()
{
	return GL_DEPTH_BUFFER_BIT;
}
#endif