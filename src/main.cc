#include <iostream>
#include <map>

#include "graphics/device.h"
#include "graphics/shader.h"
#include "graphics/renderer.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "core/camera.h"
#include "core/entity.h"
#include "core/widget.h"
#include "core/content_manager.h"
#include "graphics/material.h"
#include "graphics/directional_light.h"
#include "graphics/point_light.h"
#include "graphics/spot_light.h"
#include "graphics/postprocessing/post_process_bloom.h"
#include "graphics/postprocessing/post_process_gaussian_blur_horizontal.h"
#include "graphics/postprocessing/post_process_gaussian_blur_vertical.h"
#include "graphics/postprocessing/post_process_blend_scene_blur.h"
#include "graphics/postprocessing/post_process_tone_mapping.h"
#include "gameplay/game_controller.h"

#include "networking/network_manager.h"

#include <vector>

using namespace igad;

int bloom_effect_ids[12];
int tone_mapping_effect_ids[1];

int main(void)
{
	Device::Instance()->Startup();

	memory::FreeListAllocator* alloc = memory::MemoryManager::Instance()->ConstructAllocator<memory::FreeListAllocator>(200000);

	// Main camera
	Camera* camera = memory::MemoryManager::Allocate<Camera>(alloc, DegToRad(60), 1280.0f / 720.0f, 0.01f, 10000.0f);
	camera->SetTranslation(-15.0f, 17.0f, 17.0f);
	camera->SetRotation(0.75f, -3.14f, 0.0f);
	
	// UI camera
	Camera* ui_camera = memory::MemoryManager::Allocate<Camera>(alloc, DegToRad(60), 1280.0f / 720.0f, 0.01f, 10000.0f);
	ui_camera->SetProjectionMode(Camera::CAMERA_PROJECTION_ORTHOGRAPHIC);
	ui_camera->SetHeight(720);
	ui_camera->SetWidth(1280);
	ui_camera->SetTranslation(0, 0, 100);

	DirectionalLight* directional_light = memory::MemoryManager::Allocate<DirectionalLight>(alloc);

	directional_light->SetDirection(Vector3(0, -10, 0));
	directional_light->SetColor(Vector3(1, 1, 1));

	Device::Instance()->AddCamera(camera);
	Device::Instance()->AddUICamera(ui_camera);
	Device::Instance()->AddDirectionalLight(directional_light);

	GameController* game_controller = memory::MemoryManager::Allocate<GameController>(alloc, camera);
	Device::Instance()->AddEntity(game_controller);

	Device::Instance()->Run();

	memory::MemoryManager::Deallocate<GameController>(alloc, game_controller);
	memory::MemoryManager::Deallocate<Camera>(alloc, camera);
	memory::MemoryManager::Deallocate<Camera>(alloc, ui_camera);
	memory::MemoryManager::Deallocate<DirectionalLight>(alloc, directional_light);
	memory::MemoryManager::Instance()->DestructAllocator(alloc);

	Device::Instance()->Shutdown();
	return 0;
}

