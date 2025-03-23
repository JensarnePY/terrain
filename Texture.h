#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	GLuint unit;

	Texture(const char* image, GLuint slot, GLenum format, GLenum pixelType);
	Texture() = default;

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};
#endif