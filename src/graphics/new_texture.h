#pragma once

#include "../graphics/opengl.h"
#include <string>

namespace riko
{
	class Texture
	{
	public:
		Texture();
		
		Texture(const std::string& file_location);
		
		Texture(int width, int height);
		
		~Texture();

		void TextureFromData(GLubyte* data, bool gen_min_maps);

		GLuint GetTexture() const;
		int GetWidth() const;
		int GetHeight() const;
		int GetChannels() const;

	private:
		GLuint texture_;
		int width_;
		int height_;
		int channels_;
	};
}