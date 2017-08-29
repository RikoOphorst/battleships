#pragma once

#include "../graphics/opengl.h"
#include <string>



namespace igad
{
	/**
	* @class igad::Texture
	* @author Riko Ophorst
	* @brief Represents a texture
	*/
	class Texture
	{
	public:
		/**
		* @brief Creates an empty texture, you have to construct it yourself with Texture::TextureFromData()
		*/
		Texture();

		/**
		* @brief Creates a texture from a given file location
		* @param[in] file_location (const std::string&) the location of the texture
		*/
		Texture(const std::string& file_location);

		/**
		* @brief Creates an empty texture with pre-defined width and height, use Texture::TextureFromData()
		* to fill in the actual texture itself.
		*/
		Texture(int width, int height);

		/**
		* @brief Default Texture destructor
		*/
		~Texture();

		/**
		* @brief Given an array of bytes, creates an OpenGL texture
		* @param[in] data (GLubyte*) the data that should be used to construct the texture
		* @param[in] gen_min_maps (bool) should min maps be generated for the texture?
		*/
		void TextureFromData(GLubyte* data, bool gen_min_maps);

		/**
		* @brief Get the location of the texture
		*/
		virtual GLuint GetTexture() const;

		virtual void SetTexture(GLuint texture);

		/**
		* @brief Get the width of the texture in pixels
		*/
		int GetWidth() const;

		/**
		* @brief Get the height of the texture in pixels
		*/
		int GetHeight() const;

		/**
		* @brief Get the number of channels in the texture
		*/
		int GetChannels() const;

		/**
		* @brief hot reloads the texture
		*/
		virtual void HotReload();
	protected:
		std::string path_; //<! Path to the texture
		GLuint texture_; //<! The location of the texture
		int channels_; //<! The number of channels in the texture
		int width_; //<! The width of the texture in pixels
		int height_; //<! The height of the texture in pixels
	};
}