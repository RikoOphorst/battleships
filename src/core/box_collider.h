#pragma once

#include "../math/matrix44.h"
#include "../math/vector3.h"

namespace igad
{
	class Ray;
	struct RaycastHit;

	class BoxCollider
	{
	public:
		BoxCollider(Vector3 center, Vector3 size, Vector3 rotation);
		~BoxCollider();

		bool Raycast(const Ray& ray, float max, RaycastHit& hit_info);

		Vector3 GetLocalPosition() const;
		Matrix44 GetBoxSpace() const;
		Matrix44 GetWorldSpace() const;

		void Update();

	private:
		Vector3 center_;
		Vector3 size_;
		Vector3 rotation_;
	};
}