#include "shader_attribute.h"

#include "shader.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	ShaderAttribute::ShaderAttribute() :
		shader_(nullptr),
		name_(""),
		type_(0),
		location_(-1)
	{

	}

	//------------------------------------------------------------------------------------------------------
	ShaderAttribute::ShaderAttribute(Shader* shader, const std::string& name, GLenum type, GLint location) :
		shader_(shader),
		type_(type),
		location_(location),
		name_(name)
	{

	}

	//------------------------------------------------------------------------------------------------------
	ShaderAttribute::~ShaderAttribute()
	{
		DisableAttributePointer();
		Invalidate();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderAttribute::Reset(Shader* shader, const std::string& name, GLenum type, GLint location)
	{
		shader_ = shader;
		name_ = name;
		type_ = type;
		location_ = location;
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderAttribute::Invalidate()
	{
		shader_ = nullptr;
		name_ = "";
		type_ = 0;
		location_ = -1;
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderAttribute::SetAttributePointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
	{
		if (!IsValid())
		{
			return;
		}

		glVertexAttribPointer(
			location_,
			size,
			type,
			normalized,
			stride,
			pointer
		);
		GL_GET_ERROR();

		glEnableVertexAttribArray(location_);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void ShaderAttribute::DisableAttributePointer()
	{
		if (!IsValid())
		{
			return;
		}

		glDisableVertexAttribArray(location_);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	bool ShaderAttribute::IsValid() const
	{
		return location_ != -1;
	}

	//------------------------------------------------------------------------------------------------------
	GLenum ShaderAttribute::GetType() const
	{
		return type_;
	}

	//------------------------------------------------------------------------------------------------------
	GLint ShaderAttribute::GetLocation() const
	{
		return location_;
	}
}