#pragma once

namespace igad
{
	class Shader;
	class ShaderUniform;

	class PostProcessEffect
	{
	public:
		PostProcessEffect(Shader* shader, unsigned int number_of_outputs, unsigned int number_of_repetitions);
		virtual ~PostProcessEffect();

		Shader* GetShader();
		ShaderUniform* GetFrameBufferTexture();
		ShaderUniform* GetDepthBufferTexture();
		const unsigned int& GetNumberOfOutputs();
		const unsigned int& GetNumberOfRepetitions();

		virtual void Apply() = 0;
	protected:
		ShaderUniform* frame_buffer_texture_;
		ShaderUniform* depth_buffer_texture_;
		unsigned int number_of_outputs_;
		unsigned int number_of_repetitions_;

	private:
		Shader* shader_;
	};
}