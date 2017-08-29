#pragma once

#include "../../graphics/post_process_effect.h"

namespace igad
{
	class ShaderUniform;

	class PostProcessBloom : public PostProcessEffect
	{
	public:
		PostProcessBloom();
		~PostProcessBloom();

		void Apply() override;

	private:
		ShaderUniform* some_paramater_;
	};
}