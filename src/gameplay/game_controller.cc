#include "game_controller.h"

#include "../core/camera.h"
#include "../networking/network_manager.h"
#include "../graphics/device.h"
#include "../core/input.h"
#include <iostream>

namespace igad
{
	using namespace memory;

	//------------------------------------------------------------------------------------------------------
	GameController::GameController(Camera* camera) :
		Entity(),
		camera_(camera)
	{
		allocator_ = MemoryManager::Instance()->ConstructAllocator<FreeListAllocator>(20000);

		NetworkManager::Instance()->Startup();
		NetworkManager::Instance()->RequestClientId();
		NetworkManager::Instance()->RequestJoinGame();
		NetworkManager::Instance()->WaitForGameStart();

        current_ship_patrol_ = memory::MemoryManager::Allocate<Ship>(allocator_, Ship::ShipTypes::Patrol);
        current_ship_patrol_->SetTranslation(Vector3(-10.0f, 2.0f, 0.0f));
        current_ship_patrol_->SetScaling(5.0f, 5.0f, 5.0f);
        Device::Instance()->AddEntity(current_ship_patrol_);

        current_ship_destroyer_ = memory::MemoryManager::Allocate<Ship>(allocator_, Ship::ShipTypes::Destroyer);
        current_ship_destroyer_->SetTranslation(Vector3(-5.0f, 2.0f, 0.0f));
        current_ship_destroyer_->SetScaling(5.0f, 5.0f, 5.0f);
        Device::Instance()->AddEntity(current_ship_destroyer_);

        current_ship_submarine_ = memory::MemoryManager::Allocate<Ship>(allocator_, Ship::ShipTypes::Submarine);
        current_ship_submarine_->SetTranslation(Vector3(0.0f, 2.0f, 0.0f));
        current_ship_submarine_->SetScaling(5.0f, 5.0f, 5.0f);
        Device::Instance()->AddEntity(current_ship_submarine_);

        current_ship_battleship_ = memory::MemoryManager::Allocate<Ship>(allocator_, Ship::ShipTypes::Battleship);
        current_ship_battleship_->SetTranslation(Vector3(5.0f, 2.0f, 0.0f));
        current_ship_battleship_->SetScaling(5.0f, 5.0f, 5.0f);
        Device::Instance()->AddEntity(current_ship_battleship_);

        current_ship_carrier_ = memory::MemoryManager::Allocate<Ship>(allocator_, Ship::ShipTypes::Carrier);
        current_ship_carrier_->SetTranslation(Vector3(10.0f, 2.0f, 0.0f));
        current_ship_carrier_->SetScaling(5.0f, 5.0f, 5.0f);
        Device::Instance()->AddEntity(current_ship_carrier_);
        
        bloom_ = memory::MemoryManager::Allocate<PostProcessBloom>(allocator_);
        horizontal_blur_ = memory::MemoryManager::Allocate<PostProcessGuassianBlurHorizontal>(allocator_);
        vertical_blur_ = memory::MemoryManager::Allocate<PostProcessGuassianBlurVertical>(allocator_);
        blend_scene_blur_ = memory::MemoryManager::Allocate<PostProcessBlendSceneBlur>(allocator_);
        tone_mapping_ = memory::MemoryManager::Allocate<PostProcessToneMapping>(allocator_);

        AddBloom();
        AddToneMapping();
	}
	
	//------------------------------------------------------------------------------------------------------
	GameController::~GameController()
	{
        memory::MemoryManager::Deallocate<PostProcessBloom>(allocator_, bloom_);
        memory::MemoryManager::Deallocate<PostProcessGuassianBlurHorizontal>(allocator_, horizontal_blur_);
        memory::MemoryManager::Deallocate<PostProcessGuassianBlurVertical>(allocator_, vertical_blur_);
        memory::MemoryManager::Deallocate<PostProcessBlendSceneBlur>(allocator_, blend_scene_blur_);
        memory::MemoryManager::Deallocate<PostProcessToneMapping>(allocator_, tone_mapping_);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GameController::Update()
	{
        if (Device::Instance()->GetFullTime() > 5.0f)
        {
            camera_->TranslateBy(-Device::Instance()->GetDeltaTime() * 2.0f, 0.0f, 0.0f);
        }

        if (Input::Instance()->GetKeyState(GLFW_KEY_W) == GLFW_PRESS || Input::Instance()->GetKeyState(GLFW_KEY_W) == GLFW_REPEAT)
        {
            camera_->TranslateBy(Vector3(0.0f, 0.0f, 0.1f));
        }

        if (Input::Instance()->GetKeyState(GLFW_KEY_S) == GLFW_PRESS || Input::Instance()->GetKeyState(GLFW_KEY_S) == GLFW_REPEAT)
        {
            camera_->TranslateBy(Vector3(0.0f, 0.0f, -0.1f));
        }

        if (Input::Instance()->GetKeyState(GLFW_KEY_A) == GLFW_PRESS || Input::Instance()->GetKeyState(GLFW_KEY_A) == GLFW_REPEAT)
        {
            camera_->TranslateBy(Vector3(-0.1f, 0.0f, 0.0f));
        }

        if (Input::Instance()->GetKeyState(GLFW_KEY_D) == GLFW_PRESS || Input::Instance()->GetKeyState(GLFW_KEY_D) == GLFW_REPEAT)
        {
            camera_->TranslateBy(Vector3(0.1f, 0.0f, 0.0f));
        }

        if (Input::Instance()->GetMouseButtonState(GLFW_MOUSE_BUTTON_1) == GLFW_PRESS || Input::Instance()->GetMouseButtonState(GLFW_MOUSE_BUTTON_1) == GLFW_REPEAT)
        {
            const Vector2& mouse_delta = Input::Instance()->GetMouseDelta();
            camera_->RotateBy(Vector3(mouse_delta.y * 0.005f, mouse_delta.x * 0.005f, 0.0f));
        }

        if (Input::Instance()->GetKeyState(GLFW_KEY_V) == GLFW_PRESS)
        {
            AddBloom();
        }

        if (Input::Instance()->GetKeyState(GLFW_KEY_B) == GLFW_PRESS)
        {
            RemoveBloom();
        }

        if (Input::Instance()->GetKeyState(GLFW_KEY_N) == GLFW_PRESS)
        {
            AddToneMapping();
        }

        if (Input::Instance()->GetKeyState(GLFW_KEY_M) == GLFW_PRESS)
        {
            RemoveToneMapping();
        }

        if (Input::Instance()->GetKeyState(GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            std::cout << camera_->GetTranslation().x << ", " << camera_->GetTranslation().y << ", " << camera_->GetTranslation().z << " | " << camera_->GetRotation().x << ", " << camera_->GetRotation().y << ", " << camera_->GetRotation().z << std::endl;
        }
	}
	
	//------------------------------------------------------------------------------------------------------
	void GameController::ToggleGrids()
	{

	}

    //------------------------------------------------------------------------------------------------------
    void GameController::AddBloom()
    {
        if (bloom_added_ == false)
        {
            Device::Instance()->AddPostProcessEffect(bloom_);

            for (int i = 0; i < 5; i++)
            {
                Device::Instance()->AddPostProcessEffect(horizontal_blur_);
                Device::Instance()->AddPostProcessEffect(vertical_blur_);
            }

            Device::Instance()->AddPostProcessEffect(blend_scene_blur_);
            bloom_added_ = true;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void GameController::RemoveBloom()
    {
        if (bloom_added_ == true)
        {
            Device::Instance()->RemovePostProcessEffect(bloom_);
            for (int i = 0; i < 5; i++)
            {
                Device::Instance()->RemovePostProcessEffect(horizontal_blur_);
                Device::Instance()->RemovePostProcessEffect(vertical_blur_);
            }
            Device::Instance()->RemovePostProcessEffect(blend_scene_blur_);
            bloom_added_ = false;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void GameController::AddToneMapping()
    {
        if (tone_mapping_added_ == false)
        {
            tone_mapping_effect_ids_[0] = Device::Instance()->AddPostProcessEffect(tone_mapping_);
            tone_mapping_added_ = true;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void GameController::RemoveToneMapping()
    {
        if (tone_mapping_added_ == true)
        {
            Device::Instance()->RemovePostProcessEffect(tone_mapping_);
            tone_mapping_added_ = false;
        }
    }
}