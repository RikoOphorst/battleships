#pragma once

#include "../math/vector3.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	class PointLight
	{
	public:
		PointLight();
		PointLight(const Vector3& position, const Vector3& color, float constant_attenuation, float linear_attenuation, float quadratic_attenuation);
		~PointLight();

		const Vector3& GetPosition() const;
		const Vector3& GetColor() const;
		float GetConstantAttenuation() const;
		float GetLinearAttenuation() const;
		float GetQuadraticAttenuation() const;

		void SetPosition(const Vector3& position);
		void SetColor(const Vector3& color);
		void SetConstantAttenuation(float attenuation);
		void SetLinearAttenuation(float attenuation);
		void SetQuadraticAttenuation(float attenuation);

	private:
		Vector3 position_;
		Vector3 color_;
		float constant_attenuation_;
		float linear_attenuation_;
		float quadratic_attenuation_;
	};
}