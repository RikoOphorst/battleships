#include "new_texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../graphics/stb/stb_image.h"

namespace riko
{
	//------------------------------------------------------------------------------------------------------
	Texture::Texture() :
		width_(0),
		height_(0),
		channels_(0),
		texture_(0)
	{

	}

	//------------------------------------------------------------------------------------------------------
	Texture::Texture(const std::string& file_location)
	{
		GLubyte* data = stbi_load(file_location.c_str(), &width_, &height_, &channels_, 4);

		if (data == nullptr)
		{
			LOG("Texture at location %s could not be loaded!", file_location.c_str());
		}
		else
		{
			TextureFromData(data, true);
			stbi_image_free(data);
		}
	}

	//------------------------------------------------------------------------------------------------------
	Texture::Texture(int width, int height) :
		texture_(0),
		width_(width),
		height_(height)
	{

	}

	//------------------------------------------------------------------------------------------------------
	Texture::~Texture()
	{
		if (texture_ != 0)
		{
			glDeleteTextures(1, &texture_);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Texture::TextureFromData(GLubyte* data, bool gen_min_maps)
	{
		if (texture_ != 0)
		{
			glDeleteTextures(1, &texture_);
		}

		glGenTextures(1, &texture_);
		GL_GET_ERROR();

		glBindTexture(GL_TEXTURE_2D, texture_);
		GL_GET_ERROR();

		if (gen_min_maps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		GL_GET_ERROR();

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width_,
			height_,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data
		);
		GL_GET_ERROR();

		if (gen_min_maps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			GL_GET_ERROR();
		}
	}

	//------------------------------------------------------------------------------------------------------
	GLuint Texture::GetTexture() const
	{
		return texture_;
	}

	//------------------------------------------------------------------------------------------------------
	int Texture::GetWidth() const
	{
		return width_;
	}

	//------------------------------------------------------------------------------------------------------
	int Texture::GetHeight() const
	{
		return height_;
	}

	//------------------------------------------------------------------------------------------------------
	int Texture::GetChannels() const
	{
		return channels_;
	}
}