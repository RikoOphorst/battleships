#include "post_process_gaussian_blur_horizontal.h"

#include "../../core/content_manager.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	PostProcessGuassianBlurHorizontal::PostProcessGuassianBlurHorizontal() :
		PostProcessEffect(ContentManager::Instance()->GetShader("./assets/shaders/gaussian_blur_horizontal.vsh", "./assets/shaders/gaussian_blur_horizontal.fsh", ""), 2, 10)
	{

	}

	//------------------------------------------------------------------------------------------------------
	PostProcessGuassianBlurHorizontal::~PostProcessGuassianBlurHorizontal()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void PostProcessGuassianBlurHorizontal::Apply()
	{
		
	}
}