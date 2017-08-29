#define STB_IMAGE_IMPLEMENTATION
#include "../graphics/stb/stb_image.h"

#include "texture.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Texture::Texture() :
		width_(0),
		height_(0),
		channels_(0),
		texture_(0),
		path_("")
	{

	}

	//------------------------------------------------------------------------------------------------------
	Texture::Texture(const std::string& file_location) :
		path_(file_location)
	{
		HotReload();
	}

	//------------------------------------------------------------------------------------------------------
	Texture::Texture(int width, int height) :
		texture_(0),
		width_(width),
		height_(height),
		path_("")
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
	void Texture::SetTexture(GLuint texture)
	{
		texture_ = texture;
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
	
	//------------------------------------------------------------------------------------------------------
	void Texture::HotReload()
	{
		GLubyte* data = stbi_load(path_.c_str(), &width_, &height_, &channels_, 4);

		if (data == nullptr)
		{
			LOG("Texture at location %s could not be loaded!", path_.c_str());
		}
		else
		{
			TextureFromData(data, true);
			stbi_image_free(data);
		}
	}
}