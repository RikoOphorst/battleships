#pragma once

#include "../math/matrix44.h"
#include "../math/vector3.h"
#include "../memory/memory_manager.h"

#include <queue>
#include <vector>
#include <functional>

namespace igad
{
	class Camera;
	class Mesh;
	class Texture;
	class Renderer;
	class Shader;
	class Material;

	class Widget
	{
	public:
		Widget();
		Widget(Material* material);
		virtual ~Widget();

		virtual void Update();
		virtual void Render(Camera* camera);

		Mesh* GetMesh() const;
		Material* GetMaterial() const;
		Renderer* GetRenderer() const;

		const Matrix44& GetTransform();
		const Vector3& GetTranslation() const;
		const Vector3& GetRotation() const;
		const Vector3& GetScaling() const;
		const Vector3& GetOffset() const;

		void SetTranslation(float x, float y, float z);
		void SetTranslation(const Vector3& translation);
		void SetRotation(float x, float y, float z);
		void SetRotation(const Vector3& rotation);
		void SetScaling(float x, float y, float z);
		void SetScaling(const Vector3& up);
		void SetOffset(float x, float y, float z);
		void SetOffset(const Vector3& offset);
		void SetOnClickCallback(const std::function<void()>& callback);
		void SetOnHoverCallback(const std::function<void()>& callback);
		void SetOnLeaveCallback(const std::function<void()>& callback);

		void SetMaterial(Material* material);
		void SetRenderer(Renderer* renderer);

		void ProcessMouseInput();

		void StartSceneGraph();
		void IterateSceneGraphUpdate();
		void IterateSceneGraphRender(Camera* camera);
		void EndSceneGraph();
		void SceneGraphUpdate();
		void SceneGraphRender(Camera* camera);

		void CalcTransform();

		void SetParent(Widget* widget);
		Widget* GetParent();
		void AddChild(Widget* child);
		void RemoveChild(Widget* child);
		Widget* GetChild(unsigned int child_id) const;
	protected:
		Widget* new_parent_;
		Widget* parent_;

		std::queue<Widget*> pending_add_children_;
		std::queue<Widget*> pending_rem_children_;
		std::vector<Widget*> children_;

		Vector3 translation_;
		Vector3 rotation_;
		Vector3 scaling_;
		Vector3 offset_;

		std::function<void()> on_click_callback_;
		std::function<void()> on_hover_callback_;
		std::function<void()> on_leave_callback_;

		bool mouse_is_hovering_;

		Matrix44 world_;

		Material* material_;
		Mesh* mesh_;
		Renderer* renderer_;

		memory::LinearAllocator* alloc_;
	};
}