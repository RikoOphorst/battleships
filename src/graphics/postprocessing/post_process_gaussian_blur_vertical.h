#pragma once

#include "../../graphics/post_process_effect.h"

namespace igad
{
	class ShaderUniform;

	class PostProcessGuassianBlurVertical : public PostProcessEffect
	{
	public:
		PostProcessGuassianBlurVertical();
		~PostProcessGuassianBlurVertical();

		void Apply() override;

	private:
		ShaderUniform* some_paramater_;
	};
}