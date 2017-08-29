#include "model.h"

#include "../graphics/device.h"
#include "../core/input.h"
#include "../core/camera.h"
#include "../core/content_manager.h"
#include "../graphics/point_light.h"
#include "../graphics/spot_light.h"

#include <cmath>

namespace igad
{
	Model::Model(Camera* camera, PointLight* point_light, SpotLight* spot_light) :
		Entity(),
		camera_(camera),
		point_light_(point_light),
		spot_light_(spot_light),
		current_rotation_(0.0f)
	{
		point_light_->SetPosition(Vector3(20, 20, 20));
		point_light_->SetColor(Vector3(1, 1, 1));
		point_light_->SetConstantAttenuation(0.5f);
		point_light_->SetLinearAttenuation(0.1f);
		point_light_->SetQuadraticAttenuation(0.1f);

		spot_light_->SetPosition(Vector3(20, 20, 20));
		spot_light_->SetColor(Vector3(1, 1, 1));
		spot_light_->SetDirection(Vector3(0, -1, 0));
		spot_light_->SetInnerCone(cos(0.4f));
		spot_light_->SetOuterCone(cos(0.45f));
		spot_light_->SetConstantAttenuation(0.5f);
		spot_light_->SetLinearAttenuation(0.1f);
		spot_light_->SetQuadraticAttenuation(0.1f);

		display_allocator_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::FreeListAllocator>(sizeof(Entity) * 10);

		display1_ = memory::MemoryManager::Allocate<Entity>(display_allocator_);
		display1_->SetMesh(ContentManager::Instance()->GetMesh("./assets/models/sphere/sphere.obj"));
		display1_->SetRenderer(GetRenderer());
		display1_->SetMaterial(GetMaterial());
		display1_->SetScaling(Vector3(0.3f, 0.3f, 0.3f));

		display2_ = memory::MemoryManager::Allocate<Entity>(display_allocator_);
		display2_->SetMesh(ContentManager::Instance()->GetMesh("./assets/models/sphere/sphere.obj"));
		display2_->SetRenderer(GetRenderer());
		display2_->SetMaterial(GetMaterial());
		display2_->SetScaling(Vector3(0.3f, 0.3f, 0.3f));

		Device::Instance()->AddEntity(display1_);
		Device::Instance()->AddEntity(display2_);
	}

	Model::~Model()
	{

	}

	void Model::Update()
	{
		display1_->SetMaterial(GetMaterial());
		display2_->SetMaterial(GetMaterial());
		display1_->SetRenderer(GetRenderer());
		display2_->SetRenderer(GetRenderer());

		current_rotation_ += 0.02f;

		point_light_->SetPosition(Vector3(
			sin(current_rotation_) * 10.0f,
			cos(current_rotation_) * 10.0f,
			0
		));
		
		display1_->SetTranslation(point_light_->GetPosition() * 1.2f);

		spot_light_->SetPosition(Vector3(
			sin(current_rotation_ / 3.4f) * 8.0f,
			cos(current_rotation_ / 3.4f) * 8.0f,
			0
		));

		spot_light_->SetDirection(spot_light_->GetPosition() * -1);

		display2_->SetTranslation(spot_light_->GetPosition() * 1.2f);

		if (Input::Instance()->IsKeyDown(GLFW_KEY_W))
			camera_->TranslateBy(0, 0, 0.1f);

		if (Input::Instance()->IsKeyDown(GLFW_KEY_S))
			camera_->TranslateBy(0, 0, -0.1f);	   
												   
		if (Input::Instance()->IsKeyDown(GLFW_KEY_A))
			camera_->TranslateBy(-0.1f, 0, 0);	   
												   
		if (Input::Instance()->IsKeyDown(GLFW_KEY_D))
			camera_->TranslateBy(0.1f, 0, 0);	   
												   
		if (Input::Instance()->IsKeyDown(GLFW_KEY_Q))
			camera_->RotateBy(0.0f, 0.02f, 0.0f);  
												   
		if (Input::Instance()->IsKeyDown(GLFW_KEY_E))
			camera_->RotateBy(0.0f, -0.02f, 0.0f); 
												   
		if (Input::Instance()->IsKeyDown(GLFW_KEY_T))
			camera_->TranslateBy(0.0f, 0.2f, 0.0f);
		if (Input::Instance()->IsKeyDown(GLFW_KEY_G))
			camera_->TranslateBy(0.0f, -0.2f, 0.0f);
												   
		if (Input::Instance()->IsKeyDown(GLFW_KEY_Z))
			camera_->RotateBy(0.0f, 0.0f, 0.02f);  
		if (Input::Instance()->IsKeyDown(GLFW_KEY_X))
			camera_->RotateBy(0.0f, 0.0f, -0.02f); 
												   
		if (Input::Instance()->IsKeyDown(GLFW_KEY_R))
			camera_->RotateBy(0.02f, 0.0f, 0.0f);  
		if (Input::Instance()->IsKeyDown(GLFW_KEY_F))
			camera_->RotateBy(-0.02f, 0.0f, 0.0f);
	}
}