#pragma once

#include "../core/entity.h"
#include "../memory/memory_manager.h"

namespace igad
{
	class Camera;
	class PointLight;
	class SpotLight;

	class Model : public Entity
	{
	public:
		Model(Camera* camera, PointLight* point_light, SpotLight* spot_light);
		~Model();

		void Update();
	private:
		memory::FreeListAllocator* display_allocator_;
		Entity* display1_;
		Entity* display2_;
		Camera* camera_;
		PointLight* point_light_;
		SpotLight* spot_light_;
		float current_rotation_;
	};
}