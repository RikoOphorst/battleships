#include "post_process_tone_mapping.h"

#include "../../core/content_manager.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	PostProcessToneMapping::PostProcessToneMapping() :
		PostProcessEffect(ContentManager::Instance()->GetShader("./assets/shaders/tone_mapping.vsh", "./assets/shaders/tone_mapping.fsh", ""), 1, 10)
	{

	}

	//------------------------------------------------------------------------------------------------------
	PostProcessToneMapping::~PostProcessToneMapping()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void PostProcessToneMapping::Apply()
	{

	}
}