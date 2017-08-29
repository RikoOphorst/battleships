#include "shader_uniform.h"

#include "shader.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	ShaderUniform::ShaderUniform(Shader* shader, const std::string& name, GLenum type, GLint location, GLint sampler) :
		shader_(shader),
		name_(name),
		type_(type),
		location_(location),
		sampler_(sampler)
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	ShaderUniform::~ShaderUniform()
	{
	}

	//------------------------------------------------------------------------------------------------------
	ShaderUniform::ShaderUniform() :
		shader_(nullptr),
		name_(""),
		type_(0),
		location_(-1),
		sampler_(-1)
	{

	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::SetValue(float val)
	{
		if (!IsValid())
			return;

		ASSERT(type_ == GL_FLOAT);
		glUniform1f(location_, val);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::SetValue(int val)
	{
		if (!IsValid())
			return;

		ASSERT(type_ == GL_INT);
		glUniform1i(location_, val);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::SetValue(bool val)
	{
		if (!IsValid())
			return;

		ASSERT(type_ == GL_BOOL);
		glUniform1i(location_, val);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::SetValue(const Vector2& val)
	{
		if (!IsValid())
			return;

		ASSERT(type_ == GL_FLOAT_VEC2);
		glUniform2fv(location_, 1, &val.x);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::SetValue(const Vector3& val)
	{
		if (!IsValid())
			return;

		ASSERT(type_ == GL_FLOAT_VEC3);
		glUniform3fv(location_, 1, &val.x);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::SetValue(const Vector4& val)
	{
		if (!IsValid())
			return;

		ASSERT(type_ == GL_FLOAT_VEC4);
		glUniform4fv(location_, 1, &val.x);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::SetValue(const Color& val)
	{
		if (!IsValid())
			return;

		Vector4 c = { val.r / 255.0f, val.g / 255.0f, val.b / 255.0f, val.a / 255.0f };

		if (type_ == GL_FLOAT_VEC4)
		{
			glUniform4fv(location_, 1, &c.x);
		}
		else if (type_ == GL_FLOAT_VEC3)
		{
			glUniform3fv(location_, 1, &c.x);
		}
		else
		{
			ASSERT(false);
		}

		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::SetValue(const Matrix44& val, bool transpose)
	{
		if (!IsValid())
			return;

		ASSERT(type_ == GL_FLOAT_MAT4);
		glUniformMatrix4fv(location_, 1, transpose, val.f);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::SetValue(const Texture& texture)
	{
		if (!IsValid())
			return;

		ASSERT(type_ == GL_SAMPLER_2D || type_ == GL_SAMPLER_CUBE);
		// Use texture with index sampler. GL_TEXTURE1 = GL_TEXTURE10+1 is always true

		if (type_ == GL_SAMPLER_2D)
		{
			glActiveTexture(GL_TEXTURE0 + sampler_);
			GL_GET_ERROR();
			// Work with this texture
			glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
			GL_GET_ERROR();
			// Set the sampler
			glUniform1i(location_, sampler_);
			GL_GET_ERROR();
		}
		else
		{
			glActiveTexture(GL_TEXTURE0 + sampler_);
			GL_GET_ERROR();
			// Work with this texture
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture.GetTexture());
			GL_GET_ERROR();
			// Set the sampler
			glUniform1i(location_, sampler_);
			GL_GET_ERROR();
		}
	}

	//------------------------------------------------------------------------------------------------------
	bool ShaderUniform::IsValid()
	{
		return location_ != -1;
	}
	
	//------------------------------------------------------------------------------------------------------
	GLenum ShaderUniform::GetType() const
	{
		return type_;
	}
	
	//------------------------------------------------------------------------------------------------------
	GLint ShaderUniform::GetLocation() const
	{
		return location_;
	}

	//------------------------------------------------------------------------------------------------------
	GLint ShaderUniform::GetSampler() const
	{
		return sampler_;
	}
	
	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::Reset(Shader* shader, const std::string& name, GLenum type, GLint location, GLint sampler)
	{
		shader_ = shader;
		name_ = name;
		type_ = type;
		location_ = location;
		sampler_ = sampler;
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderUniform::Invalidate()
	{
		shader_ = nullptr;
		name_ = "";
		type_ = 0;
		location_ = -1;
		sampler_ = -1;
	}
}