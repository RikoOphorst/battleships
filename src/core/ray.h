#pragma once

#include "../math/vector3.h"

namespace igad
{
	class Ray
	{
	public:
		Ray(Vector3 end_point, Vector3 direction);

		void SetEndPoint(const Vector3& end_point);
		void SetDirection(const Vector3& direction);

		const Vector3& GetEndPoint() const;
		const Vector3& GetDirection() const;

	private:
		Vector3 end_point_;
		Vector3 direction_;
	};
}