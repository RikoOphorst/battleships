#pragma once

#include "../graphics/opengl.h"
#include "../graphics/texture.h"
#include <string>
#include <vector>

namespace igad
{
	/**
	* @class igad::TextureCube
	* @author Riko Ophorst
	* @brief Represents a TextureCube
	*/
	class TextureCube : public Texture
	{
	public:
		/**
		* @brief Creates an empty TextureCube, you have to construct it yourself with TextureCube::TextureCubeFromData()
		*/
		TextureCube();

		/**
		* @brief Creates a TextureCube from a given file location
		* @param[in] file_location (const std::string&) the location of the TextureCube
		*/
		TextureCube(const std::vector<std::string>& file_location);

		/**
		* @brief Creates an empty TextureCube with pre-defined width and height, use TextureCube::TextureCubeFromData()
		* to fill in the actual TextureCube itself.
		*/
		TextureCube(int width, int height);

		/**
		* @brief Default TextureCube destructor
		*/
		~TextureCube();

		/**
		* @brief Given an array of bytes, creates an OpenGL TextureCube
		* @param[in] data (GLubyte*) the data that should be used to construct the TextureCube
		* @param[in] gen_min_maps (bool) should min maps be generated for the TextureCube?
		*/
		void TextureCubeFromData(std::vector<GLubyte*> data, bool gen_min_maps);

		/**
		* @brief Get the location of the TextureCube
		*/
		GLuint GetTexture() const;

		void SetTexture(GLuint TextureCube);

		/**
		* @brief Get the width of the TextureCube in pixels
		*/
		int GetWidth() const;

		/**
		* @brief Get the height of the TextureCube in pixels
		*/
		int GetHeight() const;

		/**
		* @brief Get the number of channels in the TextureCube
		*/
		int GetChannels() const;

		/**
		* @brief hot reloads the TextureCube
		*/
		void HotReload();
	protected:
		std::vector<std::string> paths_; //<! Paths to the textures
		GLuint texture_; //<! The location of the texture
		int channels_; //<! The number of channels in the texture
		int width_; //<! The width of the texture in pixels
		int height_; //<! The height of the texture in pixels
	};
}