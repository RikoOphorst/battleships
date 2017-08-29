#include "directional_light.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	DirectionalLight::DirectionalLight() :
		direction_(Vector3(1, 1, 1)),
		color_(Vector3(1, 1, 1))
	{

	}

	//------------------------------------------------------------------------------------------------------
	DirectionalLight::DirectionalLight(const Vector3& direction, const Vector3& color) :
		direction_(direction),
		color_(color)
	{

	}

	//------------------------------------------------------------------------------------------------------
	DirectionalLight::~DirectionalLight()
	{

	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& DirectionalLight::GetDirection() const
	{
		return direction_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& DirectionalLight::GetColor() const
	{
		return color_;
	}

	//------------------------------------------------------------------------------------------------------
	void DirectionalLight::SetDirection(const Vector3& direction)
	{
		direction_ = direction;
	}

	//------------------------------------------------------------------------------------------------------
	void DirectionalLight::SetColor(const Vector3& color)
	{
		color_ = color;
	}
}