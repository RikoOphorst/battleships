#include "ray.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Ray::Ray(Vector3 end_point, Vector3 direction) :
		end_point_(end_point),
		direction_(direction)
	{

	}

	//------------------------------------------------------------------------------------------------------
	void Ray::SetEndPoint(const Vector3& end_point)
	{
		end_point_ = end_point;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Ray::SetDirection(const Vector3& direction)
	{
		direction_ = direction;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Ray::GetEndPoint() const
	{
		return end_point_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3 & Ray::GetDirection() const
	{
		return direction_;
	}
}