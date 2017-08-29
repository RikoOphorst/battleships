#include "Widget.h"

#include "../core/camera.h"
#include "../core/input.h"
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
	Widget::Widget() :
		material_(nullptr),
		renderer_(nullptr),
		alloc_(nullptr),
		parent_(nullptr),
		new_parent_(nullptr)
	{
		SetTranslation(0, 0, 0);
		SetRotation(0, 0, 0);
		SetScaling(1, 1, 1);
		SetOffset(0, 0, 0);

		mesh_ = Mesh::GetQuad();
	}

	//------------------------------------------------------------------------------------------------------
	Widget::Widget(Material* material) :
		material_(material),
		parent_(nullptr),
		new_parent_(nullptr)
	{
		SetTranslation(0, 0, 0);
		SetRotation(0, 0, 0);
		SetScaling(1, 1, 1);
		SetOffset(0, 0, 0);

		alloc_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::LinearAllocator>(sizeof(Renderer));
		memory::MemoryManager::Allocate<Renderer>(alloc_, material_);

		mesh_ = Mesh::GetQuad();
	}

	//------------------------------------------------------------------------------------------------------
	Widget::~Widget()
	{
		if (alloc_ != nullptr)
		{
			memory::MemoryManager::Instance()->DestructAllocator(alloc_);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::Update()
	{
		rotation_.z += 0.01f;

		ProcessMouseInput();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::Render(Camera* camera)
	{
		if (renderer_ != nullptr)
		{
			renderer_->Begin(material_, camera->GetView(), camera->GetProjection(), camera->GetTranslation());
			renderer_->Render(this);
			renderer_->End();
		}
	}

	//------------------------------------------------------------------------------------------------------
	Mesh* Widget::GetMesh() const
	{
		return mesh_;
	}

	//------------------------------------------------------------------------------------------------------
	Material* Widget::GetMaterial() const
	{
		return material_;
	}

	//------------------------------------------------------------------------------------------------------
	Renderer* Widget::GetRenderer() const
	{
		return renderer_;
	}

	//------------------------------------------------------------------------------------------------------
	const Matrix44& Widget::GetTransform()
	{
		world_ = Matrix44::TranslationFromVector(offset_) * Matrix44::ScalingFromVector(scaling_) * Matrix44::RotationFromVector(rotation_) * Matrix44::TranslationFromVector(translation_);

		if (parent_ != nullptr)
		{
			Matrix44 parent = Matrix44::TranslationFromVector(parent_->GetOffset()) * Matrix44::RotationFromVector(parent_->GetRotation()) * Matrix44::TranslationFromVector(parent_->GetTranslation());
			world_ = world_ * parent_->GetTransform();
		}

		return world_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Widget::GetTranslation() const
	{
		return translation_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Widget::GetRotation() const
	{
		return rotation_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Widget::GetScaling() const
	{
		return scaling_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Widget::GetOffset() const
	{
		return offset_;
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetTranslation(float x, float y, float z)
	{
		translation_.x = x;
		translation_.y = y;
		translation_.z = z;

		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetTranslation(const Vector3& translation)
	{
		translation_ = translation;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetRotation(float x, float y, float z)
	{
		rotation_.x = x;
		rotation_.y = y;
		rotation_.z = z;

		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetRotation(const Vector3& rotation)
	{
		rotation_ = rotation;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetScaling(float x, float y, float z)
	{
		scaling_.x = x;
		scaling_.y = y;
		scaling_.z = z;

		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetScaling(const Vector3& up)
	{
		scaling_ = up;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetOffset(float x, float y, float z)
	{
		offset_.x = x;
		offset_.y = y;
		offset_.z = z;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetOffset(const Vector3& offset)
	{
		offset_ = offset;
		CalcTransform();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetOnClickCallback(const std::function<void()>& callback)
	{
		on_click_callback_ = callback;
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetOnHoverCallback(const std::function<void()>& callback)
	{
		on_hover_callback_ = callback;
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetOnLeaveCallback(const std::function<void()>& callback)
	{
		on_leave_callback_ = callback;
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetMaterial(Material* material)
	{
		material_ = material;
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetRenderer(Renderer* renderer)
	{
		renderer_ = renderer;
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::ProcessMouseInput()
	{
		Vector2 mouse_pos = Input::Instance()->GetMousePosition();

		int width, height;
		glfwGetWindowSize(Device::Instance()->GetWindow(), &width, &height);

		mouse_pos.x -= width / 2.0f;
		mouse_pos.y -= height / 2.0f;

		float left = translation_.x + -scaling_.x / 2.0f;
		float right = translation_.x + scaling_.x / 2.0f;
		float top = translation_.y + scaling_.y / 2.0f;
		float bottom = translation_.y + -scaling_.y / 2.0f;

		if (mouse_pos.x >= left && mouse_pos.x <= right && mouse_pos.y >= bottom && mouse_pos.y <= top)
		{
			if (mouse_is_hovering_ != true)
			{
				if (on_hover_callback_ != nullptr)
				{
					on_hover_callback_();
				}
			}
			mouse_is_hovering_ = true;
		}
		else
		{
			if (mouse_is_hovering_ != false)
			{
				if (on_hover_callback_ != nullptr)
				{
					on_leave_callback_();
				}
			}
			mouse_is_hovering_ = false;
		}

		if (mouse_is_hovering_)
		{
			if (Input::Instance()->GetMouseButtonState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				if (on_hover_callback_ != nullptr)
				{
					on_click_callback_();
				}
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::StartSceneGraph()
	{
		if (new_parent_ != parent_)
			parent_ = new_parent_;

		while (!pending_add_children_.empty())
		{
			Widget* child = pending_add_children_.front();
			
			child->SetParent(this);
			children_.push_back(child);

			pending_add_children_.pop();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::IterateSceneGraphUpdate()
	{
		for (int i = 0; i < children_.size(); i++)
		{
			children_[i]->Update();
			children_[i]->IterateSceneGraphUpdate();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::IterateSceneGraphRender(Camera* camera)
	{
		for (int i = 0; i < children_.size(); i++)
		{
			children_[i]->Render(camera);
			children_[i]->IterateSceneGraphRender(camera);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::EndSceneGraph()
	{
		while (!pending_rem_children_.empty())
		{
			Widget* child = pending_rem_children_.front();
			
			for (int i = 0; i < children_.size(); i++)
			{
				if (children_[i] == child)
				{
					child->SetParent(nullptr);
					children_.erase(children_.begin() + i);
				}
				else if (i == children_.size() - 1)
				{
					BB_ASSERT(false, "Pending removal of child from scene graph did not succeed, no matches were found.");
				}
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SceneGraphUpdate()
	{
		StartSceneGraph();
		Update();
		IterateSceneGraphUpdate();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SceneGraphRender(Camera* camera)
	{
		Render(camera);
		IterateSceneGraphRender(camera);
		EndSceneGraph();
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::CalcTransform()
	{
		if (parent_ != nullptr)
		{
			world_ = parent_->GetTransform() * Matrix44::TranslationFromVector(offset_) * Matrix44::ScalingFromVector(scaling_) * Matrix44::RotationFromVector(rotation_) * Matrix44::TranslationFromVector(translation_);
		}
		else
		{
			world_ = Matrix44::TranslationFromVector(offset_) * Matrix44::ScalingFromVector(scaling_) * Matrix44::RotationFromVector(rotation_) * Matrix44::TranslationFromVector(translation_);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::SetParent(Widget* widget)
	{
		new_parent_ = widget;
		parent_ = widget;
	}

	//------------------------------------------------------------------------------------------------------
	Widget* Widget::GetParent()
	{
		return parent_;
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::AddChild(Widget* child)
	{
		pending_add_children_.push(child);
	}

	//------------------------------------------------------------------------------------------------------
	void Widget::RemoveChild(Widget* child)
	{
		pending_rem_children_.push(child);
	}

	//------------------------------------------------------------------------------------------------------
	Widget* Widget::GetChild(unsigned int child_id) const
	{
		BB_ASSERT(child_id < children_.size(), "Child id is invalid.");
		return children_[child_id];
	}
}