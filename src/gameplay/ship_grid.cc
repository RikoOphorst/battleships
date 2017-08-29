#include "ship_grid.h"

#include "../core/camera.h"
#include "../core/input.h"
#include "../core/content_manager.h"
#include "../graphics/renderer.h"
#include "../graphics/material.h"
#include "../graphics/renderer.h"
#include "../graphics/texture.h"
#include "../graphics/shader.h"
#include "../graphics/glfw/glfw3.h"
#include "../core/ray.h"
#include "../core/raycast_hit.h"
#include "../core/box_collider.h"
#include "../graphics/device.h"

#include <numeric>

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	ShipGrid::ShipGrid(Camera* camera) :
		Entity(),
		camera_(camera)
	{
		allocator_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::FreeListAllocator>(10000);

		mesh_ = ContentManager::Instance()->GetMesh("./assets/models/crate/crate.obj");

		material_ = memory::MemoryManager::Allocate<Material>(allocator_,
			ContentManager::Instance()->GetShader("./assets/shaders/lighting_default.vsh", "./assets/shaders/lighting_default.fsh", ""),
			Vector3(0.025f, 0.05f, 0.1f),
			0.2f,
			ContentManager::Instance()->GetTexture("./assets/models/crate/crate.jpg"),
			ContentManager::Instance()->GetTexture("./assets/models/crate/crate_normal.jpg"),
			ContentManager::Instance()->GetTexture("./assets/models/crate/crate_specular.jpg")
			);

		renderer_ = memory::MemoryManager::Allocate<Renderer>(allocator_, material_);

		for (int col = 0; col < 20; col++)
		{
			occupieds_.push_back(std::vector<bool>());
			for (int row = 0; row < 10; row++)
			{
				occupieds_[col].push_back(false);
			}
		}

		SetScaling(GRID_SIZE_ROWS / 2, 0.1f, GRID_SIZE_COLS / 2);

		tile_highlighter_ = memory::MemoryManager::Allocate<Entity>(allocator_);
		tile_highlighter_->SetMesh(mesh_);
		tile_highlighter_->SetMaterial(material_);
		tile_highlighter_->SetRenderer(renderer_);

		//Device::Instance()->AddEntity(tile_highlighter_);

		tile_highlighter_light_ = memory::MemoryManager::Allocate<SpotLight>(allocator_);
		tile_highlighter_light_->SetColor(Vector3(0.5f, 0.5f, 0.5f));
		tile_highlighter_light_->SetDirection(Vector3(0.0f, -1.0f, 0.0f));
		tile_highlighter_light_->SetInnerCone(cos(DegToRad(20)));
		tile_highlighter_light_->SetOuterCone(cos(DegToRad(30)));
		tile_highlighter_light_->SetConstantAttenuation(0.01f);
		tile_highlighter_light_->SetLinearAttenuation(0.001f);
		tile_highlighter_light_->SetQuadraticAttenuation(0.02f);

		Device::Instance()->AddSpotLight(tile_highlighter_light_);

		current_ship_ = nullptr;
		active_ = true;
		translate_to_ = 0.0f;
	}

	//------------------------------------------------------------------------------------------------------
	ShipGrid::~ShipGrid()
	{
		memory::MemoryManager::Deallocate<Entity>(allocator_, tile_highlighter_);
		memory::MemoryManager::Deallocate<Material>(allocator_, material_);
		memory::MemoryManager::Deallocate<Renderer>(allocator_, renderer_);
		memory::MemoryManager::Instance()->DestructAllocator(allocator_);
	}

	//------------------------------------------------------------------------------------------------------
	bool ShipGrid::CanPlaceShip(Ship* ship, unsigned int col, unsigned int row, Ship::ShipDirection direction)
	{
		switch (direction)
		{
		case Ship::ShipDirection::UP:
		{
			if ((int)row - (int)ship->GetLength() < -1)
			{
				return false;
			}
		}
			break;
		case Ship::ShipDirection::DOWN:
		{
			if ((int)row + (int)ship->GetLength() > 10)
			{
				return false;
			}
		}
			break;
		case Ship::ShipDirection::LEFT:
		{
			if ((int)col - (int)ship->GetLength() < -1)
			{
				return false;
			}
		}
			break;
		case Ship::ShipDirection::RIGHT:
		{
			if ((int)col + (int)ship->GetLength() > 20)
			{
				return false;
			}
		}
			break;
		}

		for (int i = 0; i < ship->GetLength(); i++)
		{
			if (direction == Ship::ShipDirection::UP)
			{
				if (occupieds_[col][row - i] == true)
					return false;
			}
			else if (direction == Ship::ShipDirection::DOWN)
			{
				if (occupieds_[col][row + i] == true)
					return false;
			}
			else if (direction == Ship::ShipDirection::LEFT)
			{
				if (occupieds_[col - i][row] == true)
					return false;
			}
			else if (direction == Ship::ShipDirection::RIGHT)
			{
				if (occupieds_[col + i][row] == true)
					return false;
			}
		}

		return true;
	}

	//------------------------------------------------------------------------------------------------------
	void ShipGrid::AddShip(Ship* ship, unsigned int col, unsigned int row, Ship::ShipDirection direction)
	{

		BB_ASSERT(col >= 0 && col <= 20, "Column has to be within 0-20");
		BB_ASSERT(row >= 0 && row <= 10, "Row has to be within 0-10");

		if (!CanPlaceShip(ship, col, row, direction))
			return;

		Vector3 position = Vector3();

		position.x = (int)col - 10.0f + 0.5f;
		position.y = translation_.y + 0.15f;
		position.z = -(int)row + 5.0f - 0.5f;

		ship->SetTranslation(position);
		ship->SetDirection(direction);

		ships_.push_back(ship);

		CalculateOccupieds();
	}

	//------------------------------------------------------------------------------------------------------
	void ShipGrid::RemoveShip(Ship* ship)
	{

	}

	//------------------------------------------------------------------------------------------------------
	void ShipGrid::CalculateOccupieds()
	{
		for (int row = 0; row < 10; row++)
		{
			for (int col = 0; col < 20; col++)
			{
				occupieds_[col][row] = false;
			}
		}

		for (int i = 0; i < ships_.size(); i++)
		{
			Vector2 pos = WorldPosToGrid(ships_[i]->GetTranslation());

			occupieds_[pos.x][pos.y] = true;
			
			for (int j = 1; j < ships_[i]->GetLength(); j++)
			{
				if (ships_[i]->GetDirection() == Ship::ShipDirection::UP)
				{
					occupieds_[pos.x][pos.y - j] = true;
				}
				else if (ships_[i]->GetDirection() == Ship::ShipDirection::DOWN)
				{
					occupieds_[pos.x][pos.y + j] = true;
				}
				else if (ships_[i]->GetDirection() == Ship::ShipDirection::LEFT)
				{
					occupieds_[pos.x - j][pos.y] = true;
				}
				else if (ships_[i]->GetDirection() == Ship::ShipDirection::RIGHT)
				{
					occupieds_[pos.x + j][pos.y] = true;
				}
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	Vector2 ShipGrid::WorldPosToGrid(const Vector3& position)
	{
		Vector2 grid_pos;

		grid_pos.x = std::round(position.x + 10.0f - 0.5f);
		grid_pos.y = std::round(9 - (position.z + 5.0f - 0.5f));

		return grid_pos;
	}

	//------------------------------------------------------------------------------------------------------
	void ShipGrid::UpdateTranslation(float x, float y, float z)
	{
		SetTranslation(0, y, 0);

		for (int i = 0; i < ships_.size(); i++)
		{
			ships_[i]->SetTranslation(ships_[i]->GetTranslation().x, y + 0.15f, ships_[i]->GetTranslation().z);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void ShipGrid::SetActive(bool active)
	{
		active_ = active;
	}

	//------------------------------------------------------------------------------------------------------
	void ShipGrid::SetTranslateTo(float translate_to)
	{
		translate_to_ = translate_to;
	}
	
	//------------------------------------------------------------------------------------------------------
	void ShipGrid::Update()
	{
		/*float width = 1280.0f;
		float height = 720.0f;
		Matrix44 cam_projection = camera_->GetProjection();
		Matrix44 cam_view = camera_->GetView();

		Vector3 ray_origin = camera_->GetTranslation();

		Vector2 cursor_pos = Input::Instance()->GetMousePosition();

		Vector3 ray_ndc = Vector3(
			(2.0f * cursor_pos.x) / width - 1.0f,
			1.0f - (2.0f * cursor_pos.y) / height,
			1.0f
			);

		Vector3 ray_clip = Vector3(ray_ndc.x, ray_ndc.y, 1.0f);

		cam_projection.Invert();
		Vector3 ray_eye = cam_projection * ray_clip;
		ray_eye = Vector3(ray_eye.x, ray_eye.y, 1.0);

		cam_view.Invert();
		Vector3 ray_world = cam_view.MulDirectionVector(ray_eye);
		ray_world.Normalize();

		Vector3 plane_normal = Vector3(0.0f, 1.0f, 0.0f);

		float t = -((ray_origin.Dot(plane_normal) - (scaling_.y + translation_.y)) / ray_world.Dot(plane_normal));

		Vector3 p = ray_origin + t * ray_world;

		float smallest_difference = std::numeric_limits<float>::infinity();

		float x, z;

		x = std::round(p.x);

		if (x < p.x)
		{
			x += 0.5f;
		}
		else
		{
			x -= 0.5f;
		}

		z = std::round(p.z);

		if (z < p.z)
		{
			z += 0.5f;
		}
		else
		{
			z -= 0.5f;
		}

		p.x = min(max(x, -9.5f), 9.5f);
		p.z = min(max(z, -4.5f), 4.5f);

		p.y = 0.25f;*/

		/*tile_highlighter_light_->SetPosition(Vector3(p.x, translation_.y + 3.5f, p.z));*/

		UpdateTranslation(translation_.x, Lerp(translation_.y, translate_to_, 0.075f), translation_.z);

		if (!Input::Instance()->IsKeyDown(GLFW_KEY_SPACE))
		{
			/*if (current_ship_ == nullptr)
			{
				current_ship_ = memory::MemoryManager::Allocate<Ship>(allocator_, Ship::ShipTypes::Patrol);
				Device::Instance()->AddEntity(current_ship_);
			}

			current_ship_->SetTranslation(Vector3(p.x, translation_.y + 0.15f, p.z));

			if (Input::Instance()->IsKeyDown(GLFW_KEY_1))
			{
				current_ship_->SetType(Ship::ShipTypes::Patrol);
			}
			else if (Input::Instance()->IsKeyDown(GLFW_KEY_2))
			{
				current_ship_->SetType(Ship::ShipTypes::Submarine);
			}
			else if (Input::Instance()->IsKeyDown(GLFW_KEY_3))
			{
				current_ship_->SetType(Ship::ShipTypes::Destroyer);
			}
			else if (Input::Instance()->IsKeyDown(GLFW_KEY_4))
			{
				current_ship_->SetType(Ship::ShipTypes::Battleship);
			}
			else if (Input::Instance()->IsKeyDown(GLFW_KEY_5))
			{
				current_ship_->SetType(Ship::ShipTypes::Carrier);
			}

			if (Device::Instance()->GetFullTime() - last_swap_ > 0.2)
			{
				if (Input::Instance()->IsKeyDown(GLFW_KEY_Q))
				{
					switch (current_ship_->GetDirection())
					{
					case Ship::ShipDirection::UP:
						current_ship_->SetDirection(Ship::ShipDirection::LEFT);
						break;
					case Ship::ShipDirection::LEFT:
						current_ship_->SetDirection(Ship::ShipDirection::DOWN);
						break;
					case Ship::ShipDirection::DOWN:
						current_ship_->SetDirection(Ship::ShipDirection::RIGHT);
						break;
					case Ship::ShipDirection::RIGHT:
						current_ship_->SetDirection(Ship::ShipDirection::UP);
						break;
					}

					last_swap_ = Device::Instance()->GetFullTime();
				}
				else if (Input::Instance()->IsKeyDown(GLFW_KEY_E))
				{
					switch (current_ship_->GetDirection())
					{
					case Ship::ShipDirection::UP:
						current_ship_->SetDirection(Ship::ShipDirection::RIGHT);
						break;
					case Ship::ShipDirection::RIGHT:
						current_ship_->SetDirection(Ship::ShipDirection::DOWN);
						break;
					case Ship::ShipDirection::DOWN:
						current_ship_->SetDirection(Ship::ShipDirection::LEFT);
						break;
					case Ship::ShipDirection::LEFT:
						current_ship_->SetDirection(Ship::ShipDirection::UP);
						break;
					}

					last_swap_ = Device::Instance()->GetFullTime();
				}
			}

			Vector2 grid_pos = WorldPosToGrid(current_ship_->GetTranslation());

			if (CanPlaceShip(current_ship_, grid_pos.x, grid_pos.y, current_ship_->GetDirection()) && Input::Instance()->GetMouseButtonState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				AddShip(current_ship_, grid_pos.x, grid_pos.y, current_ship_->GetDirection());

				current_ship_ = nullptr;
			}*/
		}
		else
		{
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
}