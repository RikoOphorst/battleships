#pragma once

#include "../graphics/device.h"
#include "../core/entity.h"
#include "../gameplay/ship_grid.h"
#include "../memory/memory_manager.h"
#include "../graphics/postprocessing/post_process_bloom.h"
#include "../graphics/postprocessing/post_process_gaussian_blur_horizontal.h"
#include "../graphics/postprocessing/post_process_gaussian_blur_vertical.h"
#include "../graphics/postprocessing/post_process_blend_scene_blur.h"
#include "../graphics/postprocessing/post_process_tone_mapping.h"

namespace igad
{
	class Camera;

	class GameController : public Entity
	{
	public:
		GameController(Camera* camera);
		~GameController();

		void Update();

		void ToggleGrids();

        void AddBloom();
        void RemoveBloom();
        void AddToneMapping();
        void RemoveToneMapping();

	private:
		memory::FreeListAllocator* allocator_;

		Camera* camera_;

        Ship* current_ship_battleship_;
        Ship* current_ship_patrol_;
        Ship* current_ship_carrier_;
        Ship* current_ship_submarine_;
        Ship* current_ship_destroyer_;

        PostProcessBloom* bloom_;
        PostProcessGuassianBlurHorizontal* horizontal_blur_;
        PostProcessGuassianBlurVertical* vertical_blur_;
        PostProcessBlendSceneBlur* blend_scene_blur_;
        PostProcessToneMapping* tone_mapping_;

        int bloom_effect_ids_[12];
        int tone_mapping_effect_ids_[1];

        bool bloom_added_ = false;
        bool tone_mapping_added_ = false;

		//ShipGrid* own_grid_;
		//ShipGrid* hit_grid_;
	};
}