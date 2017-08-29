#pragma once

#include "../graphics/glad/glad.h"
#include "../graphics/glfw/glfw3.h"
#include "../memory/memory_manager.h"

#include <vector>
#include <queue>

namespace igad
{
	class Camera;
	class Entity;
	class Renderer;
	class PointLight;
	class SpotLight;
	class DirectionalLight;
	class Widget;
	class PostProcessingFBO;
	class PostProcessEffect;
	class Mesh;

	class Device
	{
	public:
		Device();
		~Device();

		static Device* Instance();

		void Startup();
		void Run();
		void Shutdown();

		GLFWwindow* GetWindow();

		const std::vector<DirectionalLight*>& GetDirectionalLights();
		const std::vector<PointLight*>& GetPointLights();
		const std::vector<SpotLight*>& GetSpotLights();

		void AddCamera(Camera* camera);
		void AddUICamera(Camera* camera);
		void AddEntity(Entity* entity);
		void AddWidget(Widget* widget);
		void AddDirectionalLight(DirectionalLight* directional_light);
		void AddPointLight(PointLight* point_light);
		void AddSpotLight(SpotLight* spot_light);

		int AddPostProcessEffect(PostProcessEffect* effect);
		void RemovePostProcessEffect(PostProcessEffect* effect);
        void RemovePostProcessEffect(int index);

		void RemoveCamera(Camera* camera);
		void RemoveUICamera(Camera* camera);
		void RemoveEntity(Entity* camera);
		void RemoveWidget(Widget* widget);
		void RemoveDirectionalLight(DirectionalLight* directional_light);
		void RemovePointLight(PointLight* point_lights);
		void RemoveSpotLight(SpotLight* spot_light);

		const double& GetDeltaTime();
		const double& GetFullTime();

	private:

		double delta_time_;
		double current_time_;
		double last_time_;

		std::queue<Camera*> pending_add_cameras_;
		std::queue<Camera*> pending_add_ui_cameras_;
		std::queue<Entity*> pending_add_entities_;
		std::queue<Widget*> pending_add_widgets_;
		std::queue<DirectionalLight*> pending_add_directional_lights_;
		std::queue<PointLight*> pending_add_point_lights_;
		std::queue<SpotLight*> pending_add_spot_lights_;

		std::queue<Camera*> pending_rem_cameras_;
		std::queue<Camera*> pending_rem_ui_cameras_;
		std::queue<Entity*> pending_rem_entities_;
		std::queue<Widget*> pending_rem_widgets_;
		std::queue<DirectionalLight*> pending_rem_directional_lights_;
		std::queue<PointLight*> pending_rem_point_lights_;
		std::queue<SpotLight*> pending_rem_spot_lights_;

		std::vector<Camera*> cameras_;
		std::vector<Camera*> ui_cameras_;
		std::vector<Entity*> entities_;
		std::vector<Widget*> widgets_;
		std::vector<DirectionalLight*> directional_lights_;
		std::vector<PointLight*> point_lights_;
		std::vector<SpotLight*> spot_lights_;

		memory::StackAllocator* allocator_;
		PostProcessingFBO* post_processing_fbo_;
		std::vector<PostProcessEffect*> post_processing_effects_;

		Entity* skybox_;
		Renderer* skybox_renderer_;

		GLFWwindow* window_;
	};
}