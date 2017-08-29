#pragma once

#include "../graphics/opengl.h"

#include <iostream>
#include <map>
#include <string>

namespace igad
{
	class Shader;

	/**
	* @class igad::ShaderAttribute
	* @author Riko Ophorst
	* @brief Represents a shader attribute (i.e. per vertex data)
	*/
	class ShaderAttribute
	{
	public:
		/**
		* @brief Creates an invalid shader attribute
		*/
		ShaderAttribute();

		/**
		* @brief Creates a shader attribute representation
		* @param[in] shader (Shader*) the shader the attribute is attached to
		* @param[in] name (const std::string&) the name of the attribute in the shader
		* @param[in] type (GLenum) the type of the attribute
		* @param[in] location (GLint) the location of the attribute in memory
		*/
		ShaderAttribute(Shader* shader, const std::string& name, GLenum type, GLint location);

		/**
		* @brief Default ShaderAttribute destructor
		*/
		~ShaderAttribute();

		/**
		* @brief Resets the shader attributes so that it now represents a new attribute
		* @param[in] shader (Shader*) the shader the attribute is attached to
		* @param[in] name (const std::string&) the name of the attribute in the shader
		* @param[in] type (GLenum) the type of the attribute
		* @param[in] location (GLint) the location of the attribute in memory
		*/
		void Reset(Shader* shader, const std::string& name, GLenum type, GLint location);

		/**
		* @brief Invalidates the shader attributes, meaning it can no longer the attribute it's representing
		*/
		void Invalidate();

		/**
		* @brief Sets the attribute pointer
		* @param[in] size (GLint) the size of the memory in bytes the new pointer points to
		*/
		void SetAttributePointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

		/**
		* @brief Disables the attribute pointer
		*/
		void DisableAttributePointer();

		/**
		* @brief Is this attribute valid?
		* @note Usually, if this returns false, it means the attribute is not present in the attached shader.
		*/
		bool IsValid() const;

		/**
		* @brief Returns the type of the attribute this is representing
		*/
		GLenum GetType() const;

		/**
		* @brief Returns the location of the attribute this is representing
		*/
		GLint GetLocation() const;

	private:
		Shader* shader_; //<! the shader this attribute belongs to
		std::string name_; //<! the name of the attribute we are representing
		GLenum type_; //<! the type of the attribute we are representing
		GLint location_; //<! the location in memory of the attribute we are representing
	};
}