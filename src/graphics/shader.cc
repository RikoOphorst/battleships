#include "shader.h"

#include "../util/utils.h"

#ifdef DEBUG
#include "../memory/linear_allocator.h"
#endif

#include <vector>

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Shader::Shader()
	{
		allocator_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::FreeListAllocator>(sizeof(ShaderUniform) * 5000);
		vertex_shader_location_ = "";
		fragment_shader_location_ = "";
		geometry_shader_location_ = "";
	}

	//------------------------------------------------------------------------------------------------------
	Shader::Shader(const std::string& fragment_shader_path, const std::string& vertex_shader_path)
	{
		allocator_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::FreeListAllocator>(sizeof(ShaderUniform) * 5000);
		LoadByPath(fragment_shader_path, vertex_shader_path, "");
	}

	//------------------------------------------------------------------------------------------------------
	Shader::Shader(const std::string& fragment_shader_path, const std::string& vertex_shader_path, const std::string& geometry_shader_path)
	{
		allocator_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::FreeListAllocator>(sizeof(ShaderUniform) * 5000);
		LoadByPath(fragment_shader_path, vertex_shader_path, geometry_shader_path);
	}

	//------------------------------------------------------------------------------------------------------
	Shader::~Shader()
	{
		for (auto& i : attributes_)
		{
			memory::MemoryManager::Deallocate<ShaderAttribute>(allocator_, i.second);
		}

		for (auto& i : uniforms_)
		{
			memory::MemoryManager::Deallocate<ShaderUniform>(allocator_, i.second);
		}

		memory::MemoryManager::Instance()->DestructAllocator(allocator_);
	}

	//------------------------------------------------------------------------------------------------------
	void Shader::LoadByPath(const std::string& fragment_shader_path, const std::string& vertex_shader_path, const std::string& geometry_shader_path)
	{
		vertex_shader_location_ = vertex_shader_path;
		fragment_shader_location_ = fragment_shader_path;
		geometry_shader_location_ = geometry_shader_path;

		LoadBySource(
			igad::ReadFile(fragment_shader_path), 
			igad::ReadFile(vertex_shader_path), 
			(geometry_shader_path.length() > 0 ? igad::ReadFile(geometry_shader_path) : "")
		);
	}

	//------------------------------------------------------------------------------------------------------
	void Shader::LoadBySource(const std::string& fragment_shader_source, const std::string& vertex_shader_source, const std::string& geometry_shader_source)
	{
		GLuint fragment_shader = 0;
		GLuint vertex_shader = 0;
		GLuint geometry_shader = 0;

		program_ = glCreateProgram();

		if (!CompileShader(&fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_source.c_str()))
		{
			LOG("Shader::LoadBySource() Failed to compile fragment shader");
			ASSERT(false);
			return;
		}
		
		if (!CompileShader(&vertex_shader, GL_VERTEX_SHADER, vertex_shader_source.c_str()))
		{
			LOG("Shader::LoadBySource() Failed to compile vertex shader");
			ASSERT(false);
			return;
		}

		if (geometry_shader_source.length() > 0 && !CompileShader(&geometry_shader, GL_GEOMETRY_SHADER, geometry_shader_source.c_str()))
		{
			LOG("Shader::LoadBySource() Failed to compile geometry shader");
			ASSERT(false);
			return;
		}

		glAttachShader(program_, fragment_shader);
		glAttachShader(program_, vertex_shader);

		if (geometry_shader > 0)
		{
			glAttachShader(program_, geometry_shader);
		}
		
		if (!LinkProgram(program_))
		{
			if (vertex_shader != 0)
			{
				glDeleteShader(vertex_shader);
			}

			if (fragment_shader != 0)
			{
				glDeleteShader(fragment_shader);
			}

			if (geometry_shader != 0)
			{
				glDeleteShader(geometry_shader);
			}

			if (program_ != 0)
			{
				glDeleteProgram(program_);
			}

			fragment_shader = 0;
			vertex_shader = 0;
			geometry_shader = 0;

			GL_GET_ERROR();

			ASSERT(false);
		}


		glDeleteShader(vertex_shader);
		GL_GET_ERROR();

		glDeleteShader(fragment_shader);
		GL_GET_ERROR();

		glDeleteShader(geometry_shader);
		GL_GET_ERROR();

		LoadUniforms();
	}

	//------------------------------------------------------------------------------------------------------
	void Shader::LoadUniforms()
	{
		for (auto& itr : uniforms_)
		{
			itr.second->Invalidate();
		}

		GLint number_uniforms = 0;
		glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &number_uniforms);

		GLint max_uniform_length = 0;
		glGetProgramiv(program_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_length);

		std::vector<GLchar> uniform_name_data(max_uniform_length);

		unsigned int sampler_count = 0;
		for (int uniform = 0; uniform < number_uniforms; uniform++)
		{
			GLint array_size = 0;
			GLenum type = 0;
			GLsizei actual_length = 0;

			glGetActiveUniform(
				program_,
				uniform,
				max_uniform_length,
				&actual_length,
				&array_size,
				&type,
				&uniform_name_data[0]
				);

			std::string name(&uniform_name_data[0], actual_length);
			GLint location = glGetUniformLocation(program_, name.c_str());

			auto itr = uniforms_.find(name);
			if (itr != uniforms_.end())
			{
				if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
				{
					itr->second->Reset(this, name, type, location, sampler_count++);
				}
				else
				{
					itr->second->Reset(this, name, type, location);
				}
			}
			else
			{
				if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
				{
					uniforms_[name] = memory::MemoryManager::Allocate<ShaderUniform, Shader*, const std::string&, GLenum, GLint, int>(allocator_, this, name, type, location, sampler_count++);
				}
				else
				{
					uniforms_[name] = memory::MemoryManager::Allocate<ShaderUniform, Shader*, const std::string&, GLenum, GLint>(allocator_, this, name, type, location);
				}
			}
		}

		GLint number_attribs = 0;
		glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &number_attribs);

		GLint max_attrib_length = 0;
		glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_attrib_length);

		std::vector<GLchar> attrib_name_data(max_attrib_length);

		for (int attribute = 0; attribute < number_attribs; attribute++)
		{
			GLint array_size = 0;
			GLenum type = 0;
			GLsizei actual_length = 0;
			
			glGetActiveAttrib(
				program_,
				attribute,
				attrib_name_data.size(),
				&actual_length,
				&array_size,
				&type,
				&attrib_name_data[0]
			);

			std::string name(static_cast<char*>(&attrib_name_data[0]));
			GLint location = glGetAttribLocation(program_, name.c_str());

			auto itr = attributes_.find(name);
			if (itr != attributes_.end())
			{
				itr->second->Reset(this, name, type, location);
			}
			else
			{
				attributes_[name] = memory::MemoryManager::Allocate<ShaderAttribute, Shader*, const std::string&, GLenum, GLint>(allocator_, this, name, type, location);
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	bool Shader::CompileShader(GLuint* shader, GLenum type, const GLchar* source)
	{
		GLint status;

		if (!source)
		{
			LOG("Failed to load shader %s\n", source);
			return false;
		}

		*shader = glCreateShader(type);

		glShaderSource(*shader, 1, &source, nullptr);
		GL_GET_ERROR();

		glCompileShader(*shader);
		GL_GET_ERROR();

#ifdef DEBUG
		GLint log_length = 0;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &log_length);

		if (log_length > 1)
		{
			memory::LinearAllocator* alloc = memory::MemoryManager::Instance()->ConstructAllocator<memory::LinearAllocator>(log_length * 2);

			GLchar* log = static_cast<GLchar*>(alloc->Allocate(log_length));

			glGetShaderInfoLog(*shader, log_length, &log_length, log);
			LOG("Shader compile log: %s\n", log);

			memory::MemoryManager::Instance()->DestructAllocator(alloc);
		}
#endif

		glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			glDeleteShader(*shader);
			return false;
		}

		GL_GET_ERROR();

		return true;
	}

	//------------------------------------------------------------------------------------------------------
	bool Shader::LinkProgram(GLuint program)
	{
		GLint status;

		glLinkProgram(program);

#ifdef DEBUG
		GLint log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

		if (log_length > 1)
		{
			memory::LinearAllocator* alloc = memory::MemoryManager::Instance()->ConstructAllocator<memory::LinearAllocator>(log_length * 2);
			GLchar* log = static_cast<GLchar*>(alloc->Allocate(log_length));

			glGetShaderInfoLog(program, log_length, &log_length, log);
			LOG("Program link log: %s\n", log);

			memory::MemoryManager::Instance()->DestructAllocator(alloc);
		}
#endif

		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			return false;
		}

		GL_GET_ERROR();

		return true;
	}

	//------------------------------------------------------------------------------------------------------
	void Shader::Activate()
	{
		glUseProgram(program_);
	}

	//------------------------------------------------------------------------------------------------------
	void Shader::Deactivate()
	{
		for (auto& a : attributes_)
		{
			a.second->DisableAttributePointer();
		}
	}

	//------------------------------------------------------------------------------------------------------
	ShaderUniform* Shader::GetUniform(const std::string& name)
	{
		auto itr = uniforms_.find(name);
		if (itr != uniforms_.end())
			return itr->second;

		uniforms_[name] = memory::MemoryManager::Allocate<ShaderUniform>(allocator_);
		return uniforms_[name];
	}

	//------------------------------------------------------------------------------------------------------
	ShaderAttribute* Shader::GetAttribute(const std::string& name)
	{
		auto itr = attributes_.find(name);
		if (itr != attributes_.end())
			return itr->second;

		attributes_[name] = memory::MemoryManager::Allocate<ShaderAttribute>(allocator_);
		return attributes_[name];
	}

	//------------------------------------------------------------------------------------------------------
	GLuint Shader::GetProgram() const
	{
		return program_;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Shader::HotRelead()
	{
		LoadByPath(fragment_shader_location_, vertex_shader_location_, geometry_shader_location_);
	}
}