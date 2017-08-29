#include "box_collider.h"

#include "../core/raycast_hit.h"
#include "../core/ray.h"

#include <algorithm>

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	BoxCollider::BoxCollider(Vector3 center, Vector3 size, Vector3 rotation) :
		center_(center),
		size_(size),
		rotation_(rotation)
	{

	}

	//------------------------------------------------------------------------------------------------------
	BoxCollider::~BoxCollider()
	{

	}

	//------------------------------------------------------------------------------------------------------
	bool BoxCollider::Raycast(const Ray& ray, float max, RaycastHit& hit_info)
	{
		float xmin, xmax, ymin, ymax, zmin, zmax;
		//get a matrix that transforms from world space to box space
		Matrix44 box_space = GetBoxSpace();
		box_space.Invert();
		//calculate ray's end point and start point(yea, a ray basically becomes a line
		Vector3 ray_line_end = box_space * (ray.GetDirection().Normalized() * max + ray.GetEndPoint());
		Vector3 ray_line_start = box_space * ray.GetEndPoint();
		//line's magnitude on each axis
		Vector3 ray_max = ray_line_end - ray_line_start;

		//because we've transfered a ray into box space, box's coordinates on each axis are 0 to 1

		//calculate ray's intersections with the box on each axis. Intersections are measured in the distance from a ray's start, compared to the whole ray's length(ye, ye, ray doesn't have a length, it's a line)
		xmin = (0 - ray_line_start.x) / ray_max.x;
		xmax = (1 - ray_line_start.x) / ray_max.x;
		//this happens if the ray points from 1 to 0 instead of 0 to 1
		if (xmin > xmax)
		{
			std::swap(xmin, xmax);
		}
		//min ends up being an entry point, max - exit point

		ymin = (0 - ray_line_start.y) / ray_max.y;
		ymax = (1 - ray_line_start.y) / ray_max.y;
		if (ymin > ymax)
		{
			std::swap(ymin, ymax);
		}

		zmin = (0 - ray_line_start.z) / ray_max.z;
		zmax = (1 - ray_line_start.z) / ray_max.z;
		if (zmin > zmax)
		{
			std::swap(zmin, zmax);
		}

		//Calculate the biggest minimum and the smallest maximum, which happen to be intersections with the box on all 3 axices. 
		float maxMin(std::max(std::max(xmin, ymin), zmin));
		float minMax(std::min(std::min(xmax, ymax), zmax));

		//if ray's last entry into the box was earlier than it's first exit, it went through the box.
		//check if the hit was inside the ray's max distance
		if (maxMin < minMax && (maxMin < 1 && minMax > 0))
		{
			hit_info.is_hit = true;
			hit_info.distance = ray_max.Magnitude() * maxMin;
			hit_info.hit = ray.GetEndPoint() + ray.GetDirection().Normalized() * hit_info.distance;
			return true;
		}
		else
		{
			hit_info.is_hit = false;
			return false;
		}
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 BoxCollider::GetLocalPosition() const
	{
		return Vector3(-(size_.x) / 2 + center_.x, -(size_.y) / 2 + center_.y, -(size_.z) / 2 + center_.z);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 BoxCollider::GetBoxSpace() const
	{
		return Matrix44::ScalingFromVector(size_) * Matrix44::RotationFromVector(Vector3()) * Matrix44::TranslationFromVector(GetLocalPosition()) * Matrix44::RotationFromVector(rotation_);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 BoxCollider::GetWorldSpace() const
	{
		return Matrix44::ScalingFromVector(size_) * Matrix44::RotationFromVector(rotation_) * Matrix44::TranslationFromVector(center_);
	}

	//------------------------------------------------------------------------------------------------------
	void BoxCollider::Update()
	{

	}

}