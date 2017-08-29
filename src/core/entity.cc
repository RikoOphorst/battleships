#include "entity.h"

#include "../core/camera.h"
#include "../graphics/mesh.h"
#include "../graphics/renderer.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../graphics/glfw/glfw3.h"
#include "../graphics/device.h"
#include "../graphics/material.h"
#include "../graphics/point_light.h"

#include <iostream>

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Entity::Entity() : 
		mesh_(nullptr),
		material_(nullptr),
		renderer_(nullptr),
		alloc_(nullptr)
	{
		SetTranslation(0, 0, 0);
		SetRotation(0, 0, 0);
		SetScaling(1, 1, 1);
		SetOffset(0, 0, 0);
	}

	//------------------------------------------------------------------------------------------------------
	Entity::Entity(Material* material, Mesh* mesh) :
		mesh_(mesh),
		material_(material)
	{
		SetTranslation(0, 0, 0);
		SetRotation(0, 0, 0);
		SetScaling(1, 1, 1);
		SetOffset(0, 0, 0);

		alloc_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::LinearAllocator>(sizeof(Renderer));
		memory::MemoryManager::Allocate<Renderer>(alloc_, material_);
	}

	//------------------------------------------------------------------------------------------------------
	Entity::~Entity()
	{
		if (alloc_ != nullptr)
		{
			memory::MemoryManager::Instance()->DestructAllocator(alloc_);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::Update()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void Entity::Render(Camera* camera)
	{
		if (renderer_ != nullptr)
		{
			renderer_->Begin(material_, camera->GetView(), camera->GetProjection(), camera->GetTranslation());
			renderer_->Render(this);
			renderer_->End();
		}
	}

	//------------------------------------------------------------------------------------------------------
	Mesh* Entity::GetMesh() const
	{
		return mesh_;
	}

	//------------------------------------------------------------------------------------------------------
	Material* Entity::GetMaterial() const
	{
		return material_;
	}

	//------------------------------------------------------------------------------------------------------
	Renderer* Entity::GetRenderer() const
	{
		return renderer_;
	}

	//------------------------------------------------------------------------------------------------------
	const Matrix44& Entity::GetTransform() const
	{
		return world_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Entity::GetTranslation() const
	{
		return translation_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Entity::GetRotation() const
	{
		return rotation_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Entity::GetScaling() const
	{
		return scaling_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Entity::GetOffset() const
	{
		return offset_;
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetTranslation(float x, float y, float z)
	{
		translation_.x = x;
		translation_.y = y;
		translation_.z = z;

		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetTranslation(const Vector3& translation)
	{
		translation_ = translation;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetRotation(float x, float y, float z)
	{
		rotation_.x = x;
		rotation_.y = y;
		rotation_.z = z;

		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetRotation(const Vector3& rotation)
	{
		rotation_ = rotation;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetScaling(float x, float y, float z)
	{
		scaling_.x = x;
		scaling_.y = y;
		scaling_.z = z;

		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetScaling(const Vector3& up)
	{
		scaling_ = up;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetOffset(float x, float y, float z)
	{
		offset_.x = x;
		offset_.y = y;
		offset_.z = z;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetOffset(const Vector3& offset)
	{
		offset_ = offset;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetMesh(Mesh* mesh)
	{
		mesh_ = mesh;
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetMaterial(Material* material)
	{
		material_ = material;
	}

	//------------------------------------------------------------------------------------------------------
	void Entity::SetRenderer(Renderer* renderer)
	{
		renderer_ = renderer;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Entity::CalcTransform()
	{
		world_ = Matrix44::ScalingFromVector(scaling_) * Matrix44::TranslationFromVector(offset_) * Matrix44::RotationFromVector(rotation_) * Matrix44::TranslationFromVector(translation_);
	}
}