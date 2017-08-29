#pragma once

#include "../../graphics/post_process_effect.h"

namespace igad
{
	class ShaderUniform;

	class PostProcessToneMapping : public PostProcessEffect
	{
	public:
		PostProcessToneMapping();
		~PostProcessToneMapping();

		void Apply() override;

	private:
	};
}