#include "post_process_gaussian_blur_vertical.h"

#include "../../core/content_manager.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	PostProcessGuassianBlurVertical::PostProcessGuassianBlurVertical() :
		PostProcessEffect(ContentManager::Instance()->GetShader("./assets/shaders/gaussian_blur_vertical.vsh", "./assets/shaders/gaussian_blur_vertical.fsh", ""), 2, 10)
	{
	}

	//------------------------------------------------------------------------------------------------------
	PostProcessGuassianBlurVertical::~PostProcessGuassianBlurVertical()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void PostProcessGuassianBlurVertical::Apply()
	{
		
	}
}