#include "device.h"

#include "../memory/memory_manager.h"
#include "../util/defines.h"

#include "../core/entity.h"
#include "../core/widget.h"
#include "../core/camera.h"
#include "../core/input.h"
#include "../graphics/directional_light.h"
#include "../graphics/point_light.h"
#include "../graphics/spot_light.h"
#include "../graphics/shader.h"
#include "../graphics/renderer.h"
#include "../graphics/material.h"
#include "../graphics/mesh.h"
#include "../graphics/texture_cube.h"
#include "../graphics/post_processing_fbo.h"
#include "../core/content_manager.h"
#include "../core/camera.h"

#include "../graphics/opengl.h"

#include <iostream>

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Device::Device()
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	Device::~Device()
	{

	}

	//------------------------------------------------------------------------------------------------------
	Device* Device::Instance()
	{
		static Device* device = nullptr;
		static memory::PoolAllocator* alloc = nullptr;

		if (device == nullptr)
		{
			memory::MemoryManager::Create();
			alloc = memory::MemoryManager::Instance()->ConstructAllocator<memory::PoolAllocator, Device>(sizeof(Device));
			device = memory::MemoryManager::Allocate<Device>(alloc);
		}

		return device;
	}

	//------------------------------------------------------------------------------------------------------
	void Device::Startup()
	{
		if (!glfwInit())
		{
			BB_ASSERT(false, "Failed to initialize GLFW");
			return;
		}
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);		 // yes, 3 and 2!!!
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window_ = glfwCreateWindow(1280, 720, "Simple example", nullptr, nullptr);

		int major = glfwGetWindowAttrib(window_, GLFW_CONTEXT_VERSION_MAJOR);
		int minor = glfwGetWindowAttrib(window_, GLFW_CONTEXT_VERSION_MINOR);
		int revision = glfwGetWindowAttrib(window_, GLFW_CONTEXT_REVISION);

		std::cout << "OpenGL Version " << major << "." << minor << "." << revision << std::endl;

		if (!window_)
		{
			glfwTerminate();
			BB_ASSERT(false, "Failed to create a window");
			return;
		}

		glfwMakeContextCurrent(window_);
		glfwSwapInterval(1);
		Input::Instance()->Initialise(window_);

		if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
		{
			std::cout << "Failed to initialize OpenGL context" << std::endl;
			BB_ASSERT(false, "Failed to initialize OpenGL context");
			return;
		}

		allocator_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::StackAllocator>(sizeof(PostProcessingFBO) * 100);
		post_processing_fbo_ = memory::MemoryManager::Allocate<PostProcessingFBO>(allocator_);

		skybox_ = memory::MemoryManager::Allocate<Entity>(allocator_);
		Shader* skybox_shader = ContentManager::Instance()->GetShader("./assets/shaders/cube_map_default.vsh", "./assets/shaders/cube_map_default.fsh", "");

		TextureCube* skybox_texture = memory::MemoryManager::Allocate<TextureCube>(
			allocator_, 
			std::vector<std::string>{
				"./assets/textures/Skybox_Right.tga",
				"./assets/textures/Skybox_Left.tga",
				"./assets/textures/Skybox_Top.tga",
				"./assets/textures/Skybox_Bottom.tga",
				"./assets/textures/Skybox_Back.tga",
				"./assets/textures/Skybox_Front.tga",
			}
		);

		Material* skybox_material = memory::MemoryManager::Allocate<Material>(allocator_, skybox_shader, Vector3(0, 0, 0), 0.2f, skybox_texture, nullptr, nullptr);
		Mesh* skybox_mesh = Mesh::GetCube();
		skybox_->SetMaterial(skybox_material);
		skybox_->SetMesh(skybox_mesh);

		skybox_renderer_ = memory::MemoryManager::Allocate<Renderer>(allocator_, skybox_material);
		skybox_->SetRenderer(skybox_renderer_);
		skybox_->SetScaling(30, 30, 30);
	}

	//------------------------------------------------------------------------------------------------------
	void Device::Run()
	{
		int width, height;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		while (!glfwWindowShouldClose(window_))
		{
			glfwGetFramebufferSize(window_, &width, &height);
			glViewport(0, 0, width, height);

			current_time_ = glfwGetTime();
			delta_time_ = current_time_ - last_time_;
			last_time_ = current_time_;

			post_processing_fbo_->PrepareFBO();

			Input::Instance()->Update();
			ContentManager::Instance()->Update();

			float maximum = 0.0f;
			float current_mag = 0.0f;

			{
				while (!pending_add_cameras_.empty())
				{
					cameras_.push_back(pending_add_cameras_.front());
					pending_add_cameras_.pop();
				}

				while (!pending_add_ui_cameras_.empty())
				{
					ui_cameras_.push_back(pending_add_ui_cameras_.front());
					pending_add_ui_cameras_.pop();
				}

				while (!pending_add_entities_.empty())
				{
					entities_.push_back(pending_add_entities_.front());
					pending_add_entities_.pop();
				}

				while (!pending_add_widgets_.empty())
				{
					widgets_.push_back(pending_add_widgets_.front());
					pending_add_widgets_.pop();
				}

				while (!pending_add_directional_lights_.empty())
				{
					directional_lights_.push_back(pending_add_directional_lights_.front());
					pending_add_directional_lights_.pop();
				}

				while (!pending_add_point_lights_.empty())
				{
					point_lights_.push_back(pending_add_point_lights_.front());
					pending_add_point_lights_.pop();
				}

				while (!pending_add_spot_lights_.empty())
				{
					spot_lights_.push_back(pending_add_spot_lights_.front());
					pending_add_spot_lights_.pop();
				}

				for (int i = 0; i < cameras_.size(); i++)
				{
					float mag = cameras_[i]->GetFarZ() * 0.5f;
					skybox_->SetScaling(mag, mag, mag);
					skybox_->SetTranslation(cameras_[i]->GetTranslation());
					skybox_->Render(cameras_[i]);
				}

				for (int j = entities_.size() - 1; j >= 0; j--)
				{

					entities_[j]->Update();

					for (int i = cameras_.size() - 1; i >= 0; i--)
					{
						entities_[j]->Render(cameras_[i]);
					}
				}

				while (!pending_rem_cameras_.empty())
				{
					pending_rem_cameras_.pop();
				}

				while (!pending_rem_ui_cameras_.empty())
				{
					pending_rem_ui_cameras_.pop();
				}

				while (!pending_rem_widgets_.empty())
				{
					pending_rem_widgets_.pop();
				}

				while (!pending_rem_entities_.empty())
				{
					pending_rem_entities_.pop();
				}

				while (!pending_rem_directional_lights_.empty())
				{
					pending_rem_directional_lights_.pop();
				}

				while (!pending_rem_point_lights_.empty())
				{
					pending_rem_point_lights_.pop();
				}

				while (!pending_rem_spot_lights_.empty())
				{
					pending_rem_spot_lights_.pop();
				}
			}

			post_processing_fbo_->PreparePostProcessing();

			for (int i = 0; i < post_processing_effects_.size(); i++)
			{
				post_processing_fbo_->ApplyPostProcessEffect(post_processing_effects_[i]);
			}

			post_processing_fbo_->EndPostProcessing();

			for (int i = widgets_.size() - 1; i >= 0; i--)
			{
				widgets_[i]->SceneGraphUpdate();

				for (int j = ui_cameras_.size() - 1; j >= 0; j--)
				{
					widgets_[i]->SceneGraphRender(ui_cameras_[i]);
				}
			}

			glfwSwapBuffers(window_);
			glfwPollEvents();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Device::Shutdown()
	{
		glfwDestroyWindow(window_);
		glfwTerminate();
	}
	
	//------------------------------------------------------------------------------------------------------
	GLFWwindow* Device::GetWindow()
	{
		return window_;
	}

	//------------------------------------------------------------------------------------------------------
	const std::vector<DirectionalLight*>& Device::GetDirectionalLights()
	{
		return directional_lights_;
	}

	//------------------------------------------------------------------------------------------------------
	const std::vector<PointLight*>& Device::GetPointLights()
	{
		return point_lights_;
	}

	//------------------------------------------------------------------------------------------------------
	const std::vector<SpotLight*>& Device::GetSpotLights()
	{
		return spot_lights_;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Device::AddCamera(Camera* camera)
	{
		pending_add_cameras_.push(camera);
	}

	//------------------------------------------------------------------------------------------------------
	void Device::AddUICamera(Camera* camera)
	{
		pending_add_ui_cameras_.push(camera);
	}
	
	//------------------------------------------------------------------------------------------------------
	void Device::AddEntity(Entity* entity)
	{
		pending_add_entities_.push(entity);
	}

	//------------------------------------------------------------------------------------------------------
	void Device::AddWidget(Widget* widget)
	{
		pending_add_widgets_.push(widget);
	}

	//------------------------------------------------------------------------------------------------------
	void Device::AddDirectionalLight(DirectionalLight* directional_light)
	{
		pending_add_directional_lights_.push(directional_light);
	}

	//------------------------------------------------------------------------------------------------------
	void Device::AddPointLight(PointLight* point_light)
	{
		pending_add_point_lights_.push(point_light);
	}

	//------------------------------------------------------------------------------------------------------
	void Device::AddSpotLight(SpotLight* spot_light)
	{
		pending_add_spot_lights_.push(spot_light);
	}

	//------------------------------------------------------------------------------------------------------
	int Device::AddPostProcessEffect(PostProcessEffect* effect)
	{
		post_processing_effects_.push_back(effect);
        return post_processing_effects_.size() - 1;
	}

    //------------------------------------------------------------------------------------------------------
    void Device::RemovePostProcessEffect(PostProcessEffect* effect)
    {
        for (int i = 0; i < post_processing_effects_.size(); i++)
        {
            if (post_processing_effects_[i] == effect)
            {
                post_processing_effects_.erase(post_processing_effects_.begin() + i);
                return;
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void Device::RemovePostProcessEffect(int index)
    {
        post_processing_effects_.erase(post_processing_effects_.begin() + index);
    }
	
	//------------------------------------------------------------------------------------------------------
	void Device::RemoveCamera(Camera* camera)
	{
		pending_rem_cameras_.push(camera);
	}

	//------------------------------------------------------------------------------------------------------
	void Device::RemoveUICamera(Camera* camera)
	{
		pending_rem_ui_cameras_.push(camera);
	}
	
	//------------------------------------------------------------------------------------------------------
	void Device::RemoveEntity(Entity* entity)
	{
		pending_rem_entities_.push(entity);
	}

	//------------------------------------------------------------------------------------------------------
	void Device::RemoveWidget(Widget* widget)
	{
		pending_rem_widgets_.push(widget);
	}

	//------------------------------------------------------------------------------------------------------
	void Device::RemoveDirectionalLight(DirectionalLight* directional_light)
	{
		pending_rem_directional_lights_.push(directional_light);
	}
	
	//------------------------------------------------------------------------------------------------------
	void Device::RemovePointLight(PointLight* point_light)
	{
		pending_rem_point_lights_.push(point_light);
	}
	
	//------------------------------------------------------------------------------------------------------
	void Device::RemoveSpotLight(SpotLight* spot_light)
	{
		pending_rem_spot_lights_.push(spot_light);
	}
	
	//------------------------------------------------------------------------------------------------------
	const double& Device::GetDeltaTime()
	{
		return delta_time_;
	}
	
	//------------------------------------------------------------------------------------------------------
	const double& Device::GetFullTime()
	{
		return current_time_;
	}
}