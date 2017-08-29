#include "../graphics/stb/stb_image.h"

#include "texture_cube.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	TextureCube::TextureCube() :
		width_(0),
		height_(0),
		channels_(0),
		texture_(0)
	{

	}

	//------------------------------------------------------------------------------------------------------
	TextureCube::TextureCube(const std::vector<std::string>& file_locations) :
		paths_(file_locations)
	{
		HotReload();
	}

	//------------------------------------------------------------------------------------------------------
	TextureCube::TextureCube(int width, int height) :
		texture_(0),
		width_(width),
		height_(height)
	{

	}

	//------------------------------------------------------------------------------------------------------
	TextureCube::~TextureCube()
	{
		if (texture_ != 0)
		{
			glDeleteTextures(1, &texture_);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void TextureCube::TextureCubeFromData(std::vector<GLubyte*> data, bool gen_min_maps)
	{
		if (texture_ != 0)
		{
			glDeleteTextures(1, &texture_);
		}

		glGenTextures(1, &texture_);
		GL_GET_ERROR();

		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
		GL_GET_ERROR();

		for (int i = 0; i < data.size(); i++)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGBA,
				width_,
				height_,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				data[i]
				);
			GL_GET_ERROR();
		}

		if (gen_min_maps)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		GL_GET_ERROR();

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		GL_GET_ERROR();

		if (gen_min_maps)
		{
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			GL_GET_ERROR();
		}
	}

	//------------------------------------------------------------------------------------------------------
	GLuint TextureCube::GetTexture() const
	{
		return texture_;
	}

	//------------------------------------------------------------------------------------------------------
	void TextureCube::SetTexture(GLuint texture)
	{
		texture_ = texture;
	}

	//------------------------------------------------------------------------------------------------------
	int TextureCube::GetWidth() const
	{
		return width_;
	}

	//------------------------------------------------------------------------------------------------------
	int TextureCube::GetHeight() const
	{
		return height_;
	}

	//------------------------------------------------------------------------------------------------------
	int TextureCube::GetChannels() const
	{
		return channels_;
	}

	//------------------------------------------------------------------------------------------------------
	void TextureCube::HotReload()
	{
		std::vector<GLubyte*> all_data;
		bool failed = false;

		for (int i = 0; i < paths_.size(); i++)
		{
			all_data.push_back(stbi_load(paths_[i].c_str(), &width_, &height_, &channels_, 4));
			
			if (all_data[i] == nullptr)
			{
				LOG("Texture at location %s could not be loaded!", path_.c_str());
				failed = true;
			}
		}

		if (!failed)
		{
			TextureCubeFromData(all_data, true);
		}

		for (int i = 0; i < paths_.size(); i++)
		{
			if (all_data[i] != nullptr)
			{
				stbi_image_free(all_data[i]);
			}
		}
	}
}