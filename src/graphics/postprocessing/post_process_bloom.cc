#include "post_process_bloom.h"

#include "../../core/content_manager.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	PostProcessBloom::PostProcessBloom() :
		PostProcessEffect(ContentManager::Instance()->GetShader("./assets/shaders/bloom.vsh", "./assets/shaders/bloom.fsh", ""), 2, 1)
	{

	}

	//------------------------------------------------------------------------------------------------------
	PostProcessBloom::~PostProcessBloom()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void PostProcessBloom::Apply()
	{
		
	}
}