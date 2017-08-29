#pragma once

#include "../core/entity.h"
#include "../memory/memory_manager.h"

namespace igad
{
	class Ship : public Entity
	{
	public:
		enum ShipTypes
		{
			Carrier,
			Battleship,
			Destroyer,
			Submarine,
			Patrol
		};

		enum ShipDirection
		{
			UP = 0,
			LEFT,
			DOWN,
			RIGHT
		};

		Ship(ShipTypes ship_type);
		~Ship();

		unsigned int GetWidth();
		unsigned int GetLength();
		ShipTypes GetType();
		void SetType(ShipTypes type);
		ShipDirection GetDirection();
		void SetDirection(ShipDirection direction);

		void Update();

	private:
		memory::FreeListAllocator* allocator_;

		unsigned int width_;
		unsigned int length_;
		ShipDirection direction_;

		ShipTypes type_;

		float rotate_to_;
	};
}