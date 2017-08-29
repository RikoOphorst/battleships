#pragma once

#include "../graphics/opengl.h"
#include "../graphics/texture.h"
#include "../graphics/texture_cube.h"
#include "../math/matrix44.h"
#include "../math/vector3.h"
#include "../util/color.h"

#include <iostream>
#include <map>

namespace igad
{
	class Shader;

	/**
	* @class igad::ShaderUniform
	* @author Riko Ophorst
	* @brief Represents a uniform variable inside a shader
	*/
	class ShaderUniform
	{
	public:
		/**
		* @brief Creates an invalid shader uniform
		*/
		ShaderUniform();

		/**
		* @brief Creates a shader uniform representation based on given arguments
		* @param[in] shader (Shader*) the shader this uniform belongs to
		* @param[in] name (const std::string&) the identifier of the variable in the shader
		* @param[in] type (GLenum) the type of uniform (e.g. float, vector, matrix)
		* @param[in] location (GLint) the location in memory of the uniform
		* @param[in] sampler (GLint) optional, defines the sampler slot
		*/
		ShaderUniform(Shader* shader, const std::string& name, GLenum type, GLint location, GLint sampler = -1);

		/**
		* @brief Default ShaderUniform destructor
		*/
		~ShaderUniform();
		
		/**
		* @brief Sets this uniform's value
		* @param[in] val (float) the value
		*/
		void SetValue(float val);

		/**
		* @brief Sets this uniform's value
		* @param[in] val (int) the value
		*/
		void SetValue(int val);

		/**
		* @brief Sets this uniform's value
		* @param[in] val (bool) the value
		*/
		void SetValue(bool val);

		/**
		* @brief Sets this uniform's value
		* @param[in] val (const Vector2&) the value
		*/
		void SetValue(const Vector2& val);

		/**
		* @brief Sets this uniform's value
		* @param[in] val (const Vector3&) the value
		*/
		void SetValue(const Vector3& val);

		/**
		* @brief Sets this uniform's value
		* @param[in] val (const Vector4&) the value
		*/
		void SetValue(const Vector4& val);

		/**
		* @brief Sets this uniform's value
		* @param[in] val (const Color&) the value
		*/
		void SetValue(const Color& val);

		/**
		* @brief Sets this uniform's value
		* @param[in] val (const Matrix44&) the value
		* @param[in] transpose (bool) should the matrix be transposed
		*/
		void SetValue(const Matrix44& val, bool transpose = false);

		/**
		* @brief Sets this uniform's value
		* @param[in] val (const Texture&) the value
		*/
		void SetValue(const Texture& texture);

		/**
		* @brief Is this uniform valid?
		* @note Usually, if this returns false, it means the uniform is not present in the attached shader.
		*/
		bool IsValid();

		/**
		* @brief Returns the type of this uniform
		*/
		GLenum GetType() const;

		/**
		* @brief Returns the location in memory of this uniform
		*/
		GLint GetLocation() const;

		GLint GetSampler() const;

		/**
		* @brief Resets the uniforms so that it represents something else
		* @param[in] shader (Shader*) the shader this uniform belongs to
		* @param[in] name (const std::string&) the identifier of the variable in the shader
		* @param[in] type (GLenum) the type of uniform (e.g. float, vector, matrix)
		* @param[in] location (GLint) the location in memory of the uniform
		* @param[in] sampler (GLint) optional, defines the sampler slot
		*/
		void Reset(Shader* shader, const std::string& name, GLenum type, GLint location, GLint sampler = -1);

		/**
		* @brief This resets all values in this object so that it can no longer be used to set values in a shader
		*/
		void Invalidate();

	private:
		Shader* shader_; //<! The shader this ShaderUniform is attached to
		std::string name_; //<! The name of the uniform we are representing
		GLenum type_; //<! The type of the uniform we are representing
		GLint location_; //<! The location in memory of the uniform we are representing
		GLint sampler_; //<! The sampler id of the uniform we are representing
	};
}