#pragma once

#include "../graphics/opengl.h"
#include "../graphics/shader_uniform.h"
#include "../graphics/shader_attribute.h"
#include "../memory/memory_manager.h"
#include "../memory/free_list_allocator.h"

#include <iostream>
#include <map>

namespace igad
{
	/**
	* @class igad::Shader
	* @author Riko Ophorst
	* @brief Represents a shader
	* @note Technically, this class represents a program, not a shader
	*/
	class Shader
	{
	public:
		/**
		* @brief Creates an invalid shader
		*/
		Shader();

		/**
		* @brief Constructs a shader from a fragment & vertex shader file location
		* @param[in] fragment_shader_path (const std::string&) the path to the frag shader
		* @param[in] vertex_shader_path (const std::string&) the path to the vertex shader
		*/
		Shader(const std::string& fragment_shader_path, const std::string& vertex_shader_path);

		/**
		* @brief Constructs a shader from a fragment, vertex & geometry shader file location
		* @param[in] fragment_shader_path (const std::string&) the path to the frag shader
		* @param[in] vertex_shader_path (const std::string&) the path to the vertex shader
		* @param[in] geometry_shader_path (const std::string&) the path to the geometry shader
		*/
		Shader(const std::string& fragment_shader_path, const std::string& vertex_shader_path, const std::string& geometry_shader_path);

		/**
		* @brief Default Shader destructor
		*/
		~Shader();

		/**
		* @brief Loads a shader program by given paths
		* @param[in] fragment_shader_path (const std::string&) the path to the frag shader
		* @param[in] vertex_shader_path (const std::string&) the path to the vertex shader
		* @param[in] geometry_shader_path (const std::string&) the path to the geometry shader
		*/
		void LoadByPath(const std::string& fragment_shader_path, const std::string& vertex_shader_path, const std::string& geometry_shader_path);

		/**
		* @brief Loads a shader program by given source codes
		* @param[in] fragment_shader (const std::string&) the source of the frag shader
		* @param[in] vertex_shader (const std::string&) the source of the vertex shader
		* @param[in] geometry_shader (const std::string&) the source of the geometry shader
		*/
		void LoadBySource(const std::string& fragment_shader, const std::string& vertex_shader, const std::string& geometry_shader);

		/**
		* @brief Loads representations for all uniforms and attributes
		*/
		void LoadUniforms();

		/**
		* @brief Binds this shader program to the graphics pipeline
		*/
		void Activate();

		/**
		* @brief Unbinds this shader program from the graphics pipeline
		*/
		void Deactivate();

		/**
		* @brief Returns a shader uniform that corresponds with a given name
		* @param[in] name (const std::string&) the identifier of the uniform that you want
		*/
		ShaderUniform* GetUniform(const std::string& name);

		/**
		* @brief Returns a shader attribute that corresponds with a given name
		* @param[in] name (const std::string&) the identifier of the attribute that you want
		*/
		ShaderAttribute* GetAttribute(const std::string& name);

		/**
		* @brief Get the actual program
		*/
		GLuint GetProgram() const;

		/**
		* @brief Refrehes the shader program
		*/
		void HotRelead();

		/**
		* @brief Compiles a shader by source code
		* @param[in] shader (GLuint*) the shader in which the compiled source should be placed
		* @param[in] type (GLenum) the type of shader (e.g. GL_VERTEX_SHADER, GL_FRAGMENT_SHADER & GL_GEOMETRY_SHADER)
		* @param[in] source (GLchar*) the source code of the shader
		*/
		static bool CompileShader(GLuint* shader, GLenum type, const GLchar* source);

		/**
		* @brief Links a shader program
		* @param[in] program (GLuint) the program that should be linked
		*/
		static bool LinkProgram(GLuint program);
	private:
		std::string vertex_shader_location_;
		std::string fragment_shader_location_;
		std::string geometry_shader_location_;

		std::map<std::string, ShaderUniform*> uniforms_; //<! A map containing all uniforms that are in this shader
		std::map<std::string, ShaderAttribute*> attributes_; //<! A map containing all attributes that are in this shader

		memory::FreeListAllocator* allocator_; //<! An allocator used to allocate all uniforms and attributes

		GLuint program_; //<! The actual program
	};
}