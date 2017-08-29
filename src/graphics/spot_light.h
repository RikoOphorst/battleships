#pragma once

#include "../math/vector3.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	class SpotLight
	{
	public:
		SpotLight();
		SpotLight(const Vector3& position, const Vector3& color, const Vector3& direction, float inner_cone, float outer_cone, float constant_attenuation, float linear_attenuation, float quadratic_attenuation);
		~SpotLight();

		const Vector3& GetPosition() const;
		const Vector3& GetColor() const;
		const Vector3& GetDirection() const;
		float GetInnerCone() const;
		float GetOuterCone() const;
		float GetConstantAttenuation() const;
		float GetLinearAttenuation() const;
		float GetQuadraticAttenuation() const;

		void SetPosition(const Vector3& position);
		void SetColor(const Vector3& color);
		void SetDirection(const Vector3& direction);
		void SetInnerCone(float angle);
		void SetOuterCone(float angle);
		void SetConstantAttenuation(float attenuation);
		void SetLinearAttenuation(float attenuation);
		void SetQuadraticAttenuation(float attenuation);
	private:
		Vector3 position_;
		Vector3 color_;
		Vector3 direction_;
		float inner_cone_;
		float outer_cone_;
		float constant_attenuation_;
		float linear_attenuation_;
		float quadratic_attenuation_;
	};
}