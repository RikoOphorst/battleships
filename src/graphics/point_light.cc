#include "point_light.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	PointLight::PointLight() :
		position_(0, 0, 0),
		color_(1, 1, 1),
		constant_attenuation_(0.5f),
		linear_attenuation_(0.5f),
		quadratic_attenuation_(0.5f)
	{

	}

	//------------------------------------------------------------------------------------------------------
	PointLight::PointLight(const Vector3& position, const Vector3& color, float constant_attenuation, float linear_attenuation, float quadratic_attenuation) :
		position_(position),
		color_(color),
		constant_attenuation_(constant_attenuation),
		linear_attenuation_(linear_attenuation),
		quadratic_attenuation_(quadratic_attenuation)
	{

	}

	//------------------------------------------------------------------------------------------------------
	PointLight::~PointLight()
	{

	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& PointLight::GetPosition() const
	{
		return position_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& PointLight::GetColor() const
	{
		return color_;
	}

	//------------------------------------------------------------------------------------------------------
	float PointLight::GetConstantAttenuation() const
	{
		return constant_attenuation_;
	}

	//------------------------------------------------------------------------------------------------------
	float PointLight::GetLinearAttenuation() const
	{
		return linear_attenuation_;
	}

	//------------------------------------------------------------------------------------------------------
	float PointLight::GetQuadraticAttenuation() const
	{
		return quadratic_attenuation_;
	}

	//------------------------------------------------------------------------------------------------------
	void PointLight::SetPosition(const Vector3& position)
	{
		position_ = position;
	}

	//------------------------------------------------------------------------------------------------------
	void PointLight::SetColor(const Vector3& color)
	{
		color_ = color;
	}

	//------------------------------------------------------------------------------------------------------
	void PointLight::SetConstantAttenuation(float attenuation)
	{
		constant_attenuation_ = attenuation;
	}

	//------------------------------------------------------------------------------------------------------
	void PointLight::SetLinearAttenuation(float attenuation)
	{
		linear_attenuation_ = attenuation;
	}

	//------------------------------------------------------------------------------------------------------
	void PointLight::SetQuadraticAttenuation(float attenuation)
	{
		quadratic_attenuation_ = attenuation;
	}
}