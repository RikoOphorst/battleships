#include "post_process_blend_scene_blur.h"

#include "../../core/content_manager.h"
#include "../../graphics/shader.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	PostProcessBlendSceneBlur::PostProcessBlendSceneBlur() :
		PostProcessEffect(ContentManager::Instance()->GetShader("./assets/shaders/blend_scene_blur.vsh", "./assets/shaders/blend_scene_blur.fsh", ""), 1, 1)
	{
		Shader* shader = GetShader();
		some_paramater_ = shader->GetUniform("u_some_parameter");
	}

	//------------------------------------------------------------------------------------------------------
	PostProcessBlendSceneBlur::~PostProcessBlendSceneBlur()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void PostProcessBlendSceneBlur::Apply()
	{

	}
}