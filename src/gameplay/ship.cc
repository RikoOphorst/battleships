#include "ship.h"

#include "../core/camera.h"
#include "../core/input.h"
#include "../core/content_manager.h"
#include "../graphics/renderer.h"
#include "../graphics/material.h"
#include "../graphics/renderer.h"
#include "../graphics/texture.h"
#include "../graphics/shader.h"
#include "../graphics/glfw/glfw3.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Ship::Ship(Ship::ShipTypes ship_type) :
		Entity()
	{
		allocator_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::FreeListAllocator>(10000);

		std::string mesh;
		std::string texture;

		width_ = 1;

		switch (ship_type)
		{
		case ShipTypes::Carrier:
			mesh = "./assets/models/carrier/Carrier.obj";
			texture = "./assets/models/carrier/Carrier.tga";
			SetOffset(0.0f, 0.0f, 2.0f);
			length_ = 5;
			break;
		case ShipTypes::Battleship:
			mesh = "./assets/models/battleship/Battleship.obj";
			texture = "./assets/models/battleship/Battleship.tga";
			SetOffset(0.0f, 0.0f, 1.5f);
			length_ = 4;
			break;
		case ShipTypes::Destroyer:
			mesh = "./assets/models/destroyer/Destroyer.obj";
			texture = "./assets/models/destroyer/Destroyer.tga";
			SetOffset(0.0f, 0.0f, 1.0f);
			length_ = 3;
			break;
		case ShipTypes::Submarine:
			mesh = "./assets/models/submarine/Submarine.obj";
			texture = "./assets/models/submarine/Submarine.tga";
			SetOffset(0.0f, 0.0f, 1.0f);
			length_ = 3;
			break;
		case ShipTypes::Patrol:
			mesh = "./assets/models/patrol/Patrol.obj";
			texture = "./assets/models/patrol/Patrol.tga";
			SetOffset(0.0f, 0.0f, 0.5f);
			length_ = 2;
			break;
		}

		mesh_ = ContentManager::Instance()->GetMesh(mesh);

		material_ = memory::MemoryManager::Allocate<Material>(allocator_,
			ContentManager::Instance()->GetShader("./assets/shaders/lighting_default.vsh", "./assets/shaders/lighting_default.fsh", ""),
			Vector3(0.025f, 0.05f, 0.1f),
			0.1f,
			ContentManager::Instance()->GetTexture(texture),
			nullptr,
			nullptr
		);

		renderer_ = memory::MemoryManager::Allocate<Renderer>(allocator_, material_);
		rotate_to_ = 0;

		SetDirection(Ship::ShipDirection::UP);

		type_ = ship_type;
	}

	//------------------------------------------------------------------------------------------------------
	Ship::~Ship()
	{
		memory::MemoryManager::Deallocate<Material>(allocator_, material_);
		memory::MemoryManager::Deallocate<Renderer>(allocator_, renderer_);
		memory::MemoryManager::Instance()->DestructAllocator(allocator_);
	}
	
	//------------------------------------------------------------------------------------------------------
	unsigned int Ship::GetWidth()
	{
		return width_;
	}

	//------------------------------------------------------------------------------------------------------
	unsigned int Ship::GetLength()
	{
		return length_;
	}
	
	//------------------------------------------------------------------------------------------------------
	Ship::ShipTypes Ship::GetType()
	{
		return type_;
	}

	//------------------------------------------------------------------------------------------------------
	void Ship::SetType(ShipTypes type)
	{
		std::string mesh;
		std::string texture;

		width_ = 1;

		switch (type)
		{
		case ShipTypes::Carrier:
			mesh = "./assets/models/carrier/Carrier.obj";
			texture = "./assets/models/carrier/Carrier.tga";
			SetOffset(0.0f, 0.0f, 2.0f);
			length_ = 5;
			break;
		case ShipTypes::Battleship:
			mesh = "./assets/models/battleship/Battleship.obj";
			texture = "./assets/models/battleship/Battleship.tga";
			SetOffset(0.0f, 0.0f, 1.5f);
			length_ = 4;
			break;
		case ShipTypes::Destroyer:
			mesh = "./assets/models/destroyer/Destroyer.obj";
			texture = "./assets/models/destroyer/Destroyer.tga";
			SetOffset(0.0f, 0.0f, 1.0f);
			length_ = 3;
			break;
		case ShipTypes::Submarine:
			mesh = "./assets/models/submarine/Submarine.obj";
			texture = "./assets/models/submarine/Submarine.tga";
			SetOffset(0.0f, 0.0f, 1.0f);
			length_ = 3;
			break;
		case ShipTypes::Patrol:
			mesh = "./assets/models/patrol/Patrol.obj";
			texture = "./assets/models/patrol/Patrol.tga";
			SetOffset(0.0f, 0.0f, 0.5f);
			length_ = 2;
			break;
		}

		mesh_ = ContentManager::Instance()->GetMesh(mesh);
		material_->SetDiffuseMap(ContentManager::Instance()->GetTexture(texture));

		type_ = type;
	}
	
	//------------------------------------------------------------------------------------------------------
	Ship::ShipDirection Ship::GetDirection()
	{
		return direction_;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Ship::SetDirection(Ship::ShipDirection direction)
	{
		SetRotation(0, rotate_to_, 0);
		switch (direction_)
		{
		case ShipDirection::UP:

			if (direction == LEFT)
				rotate_to_ -= Pi * 0.5f;
			if (direction == DOWN)
				rotate_to_ += Pi;
			if (direction == RIGHT)
				rotate_to_ += Pi * 0.5f;

			break;
		case ShipDirection::DOWN:

			if (direction == LEFT)
				rotate_to_ += Pi * 0.5f;
			if (direction == UP)
				rotate_to_ += Pi;
			if (direction == RIGHT)
				rotate_to_ -= Pi * 0.5f;

			break;
		case ShipDirection::LEFT:

			if (direction == UP)
				rotate_to_ += Pi * 0.5f;
			if (direction == RIGHT)
				rotate_to_ += Pi;
			if (direction == DOWN)
				rotate_to_ -= Pi * 0.5f;

			break;
		case ShipDirection::RIGHT:

			if (direction == UP)
				rotate_to_ -= Pi * 0.5f;
			if (direction == LEFT)
				rotate_to_ += Pi;
			if (direction == DOWN)
				rotate_to_ += Pi * 0.5f;

			break;
		}

		direction_ = direction;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Ship::Update()
	{
		SetRotation(rotation_.x, Lerp<float>(rotation_.y, rotate_to_, 0.1f), rotation_.z);
	}
}