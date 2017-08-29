#pragma once

#include "../core/entity.h"
#include "../memory/memory_manager.h"
#include "../gameplay/ship.h"
#include "../graphics/spot_light.h"

#include <vector>

#define GRID_SIZE_ROWS 20
#define GRID_SIZE_COLS 10

namespace igad
{
	class Camera;

	class ShipGrid : public Entity
	{
	public:
		ShipGrid(Camera* camera);
		~ShipGrid();

		bool CanPlaceShip(Ship* ship, unsigned int col, unsigned int row, Ship::ShipDirection direction);
		void AddShip(Ship* ship, unsigned int col, unsigned int row, Ship::ShipDirection direction);
		void RemoveShip(Ship* ship);

		void CalculateOccupieds();

		Vector2 WorldPosToGrid(const Vector3& pos);
		
		void UpdateTranslation(float x, float y, float z);

		void SetActive(bool active);
		void SetTranslateTo(float translate_to);

		void Update();

	private:
		memory::FreeListAllocator* allocator_;

		Camera* camera_;
		std::vector<Ship*> ships_;
		std::vector<std::vector<bool>> occupieds_;
		std::vector<std::vector<bool>> hits_;

		Entity* tile_highlighter_;
		SpotLight* tile_highlighter_light_;

		Ship* current_ship_;
		double last_swap_;

		float translate_to_;

		bool active_;
	};
}