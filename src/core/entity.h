#pragma once

#include "../math/matrix44.h"
#include "../math/vector3.h"
#include "../memory/memory_manager.h"

namespace igad
{
	class Camera;
	class Mesh;
	class Texture;
	class Renderer;
	class Shader;
	class Material;

	class Entity
	{
	public:
		Entity();
		Entity(Material* material, Mesh* mesh);
		virtual ~Entity();

		virtual void Update();
		virtual void Render(Camera* camera);

		Mesh* GetMesh() const;
		Material* GetMaterial() const;
		Renderer* GetRenderer() const;

		const Matrix44& GetTransform() const;
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

		void SetMesh(Mesh* mesh);
		void SetMaterial(Material* material);
		void SetRenderer(Renderer* renderer);

		void CalcTransform();
	protected:
		Vector3 translation_;
		Vector3 rotation_;
		Vector3 scaling_;
		Vector3 offset_;
		
		Matrix44 world_;

		Material* material_;
		Mesh* mesh_;
		Renderer* renderer_;

		memory::LinearAllocator* alloc_;
	};
}