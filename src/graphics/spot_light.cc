#include "spot_light.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	SpotLight::SpotLight() :
		position_(0, 0, 0),
		color_(1, 1, 1),
		direction_(1.0f, 0.0f, 0.0f),
		inner_cone_(0.9f),
		outer_cone_(0.95f),
		constant_attenuation_(0.5f),
		linear_attenuation_(0.5f),
		quadratic_attenuation_(0.5f)
	{

	}

	//------------------------------------------------------------------------------------------------------
	SpotLight::SpotLight(const Vector3& position, const Vector3& color, const Vector3& direction, float inner_cone, float outer_cone, float constant_attenuation, float linear_attenuation, float quadratic_attenuation) :
		position_(position),
		color_(color),
		direction_(direction.Normalized()),
		inner_cone_(inner_cone),
		outer_cone_(outer_cone),
		constant_attenuation_(constant_attenuation),
		linear_attenuation_(linear_attenuation),
		quadratic_attenuation_(quadratic_attenuation)
	{

	}

	//------------------------------------------------------------------------------------------------------
	SpotLight::~SpotLight()
	{

	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& SpotLight::GetPosition() const
	{
		return position_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& SpotLight::GetColor() const
	{
		return color_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3 & SpotLight::GetDirection() const
	{
		return direction_;
	}

	//------------------------------------------------------------------------------------------------------
	float SpotLight::GetInnerCone() const
	{
		return inner_cone_;
	}

	//------------------------------------------------------------------------------------------------------
	float SpotLight::GetOuterCone() const
	{
		return outer_cone_;
	}

	//------------------------------------------------------------------------------------------------------
	float SpotLight::GetConstantAttenuation() const
	{
		return constant_attenuation_;
	}

	//------------------------------------------------------------------------------------------------------
	float SpotLight::GetLinearAttenuation() const
	{
		return linear_attenuation_;
	}

	//------------------------------------------------------------------------------------------------------
	float SpotLight::GetQuadraticAttenuation() const
	{
		return quadratic_attenuation_;
	}

	//------------------------------------------------------------------------------------------------------
	void SpotLight::SetPosition(const Vector3& position)
	{
		position_ = position;
	}

	//------------------------------------------------------------------------------------------------------
	void SpotLight::SetColor(const Vector3& color)
	{
		color_ = color;
	}

	//------------------------------------------------------------------------------------------------------
	void SpotLight::SetDirection(const Vector3& direction)
	{
		direction_ = direction;
	}

	//------------------------------------------------------------------------------------------------------
	void SpotLight::SetInnerCone(float angle)
	{
		inner_cone_ = angle;
	}

	//------------------------------------------------------------------------------------------------------
	void SpotLight::SetOuterCone(float angle)
	{
		outer_cone_ = angle;
	}

	//------------------------------------------------------------------------------------------------------
	void SpotLight::SetConstantAttenuation(float attenuation)
	{
		constant_attenuation_ = attenuation;
	}

	//------------------------------------------------------------------------------------------------------
	void SpotLight::SetLinearAttenuation(float attenuation)
	{
		linear_attenuation_ = attenuation;
	}

	//------------------------------------------------------------------------------------------------------
	void SpotLight::SetQuadraticAttenuation(float attenuation)
	{
		quadratic_attenuation_ = attenuation;
	}
}