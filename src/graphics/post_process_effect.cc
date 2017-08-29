#include "post_process_effect.h"

#include "../graphics/shader.h"
#include "../graphics/shader_uniform.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	PostProcessEffect::PostProcessEffect(Shader* shader, unsigned int number_of_outputs, unsigned int number_of_repetitions) :
		shader_(shader),
		number_of_outputs_(number_of_outputs),
		number_of_repetitions_(number_of_repetitions)
	{
		frame_buffer_texture_ = shader_->GetUniform("u_frame_buffer_texture");
		depth_buffer_texture_ = shader_->GetUniform("u_depth_buffer_texture");
	}

	//------------------------------------------------------------------------------------------------------
	PostProcessEffect::~PostProcessEffect()
	{

	}

	//------------------------------------------------------------------------------------------------------
	Shader* PostProcessEffect::GetShader()
	{
		return shader_;
	}
	
	//------------------------------------------------------------------------------------------------------
	ShaderUniform* PostProcessEffect::GetFrameBufferTexture()
	{
		return frame_buffer_texture_;
	}
	//------------------------------------------------------------------------------------------------------
	ShaderUniform* PostProcessEffect::GetDepthBufferTexture()
	{
		return depth_buffer_texture_;
	}
	
	//------------------------------------------------------------------------------------------------------
	const unsigned int& PostProcessEffect::GetNumberOfOutputs()
	{
		return number_of_outputs_;
	}
	
	//------------------------------------------------------------------------------------------------------
	const unsigned int& PostProcessEffect::GetNumberOfRepetitions()
	{
		return number_of_repetitions_;
	}
}