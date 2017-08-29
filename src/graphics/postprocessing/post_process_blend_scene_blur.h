#pragma once

#include "../../graphics/post_process_effect.h"

namespace igad
{
	class ShaderUniform;

	class PostProcessBlendSceneBlur : public PostProcessEffect
	{
	public:
		PostProcessBlendSceneBlur();
		~PostProcessBlendSceneBlur();

		void Apply() override;

	private:
		ShaderUniform* some_paramater_;
	};
}