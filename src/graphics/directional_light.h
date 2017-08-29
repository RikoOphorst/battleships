#pragma once

#include "../math/vector3.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	class DirectionalLight
	{
	public:
		DirectionalLight();
		DirectionalLight(const Vector3& direction, const Vector3& color);
		~DirectionalLight();

		const Vector3& GetDirection() const;
		const Vector3& GetColor() const;

		void SetDirection(const Vector3& direction);
		void SetColor(const Vector3& color);

	private:
		Vector3 direction_;
		Vector3 color_;
	};
}