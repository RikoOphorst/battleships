#pragma once

#include "../../graphics/post_process_effect.h"

namespace igad
{
	class ShaderUniform;

	class PostProcessGuassianBlurHorizontal : public PostProcessEffect
	{
	public:
		PostProcessGuassianBlurHorizontal();
		~PostProcessGuassianBlurHorizontal();

		void Apply() override;

	private:
		ShaderUniform* some_paramater_;
	};
}