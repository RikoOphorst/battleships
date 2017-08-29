#include "vector3.h"

namespace igad
{
	Vector3::Vector3() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{

	}

	Vector3::Vector3(float x, float y, float z) :
		x(x),
		y(y),
		z(z)
	{

	}

	Vector3 Vector3::operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 Vector3::operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 Vector3::operator*(const float other) const
	{
		return Vector3(x * other, y * other, z * other);
	}

	Vector3 Vector3::operator/(const float other) const
	{
		return Vector3(x / other, y / other, z / other);
	}

	Vector3 Vector3::operator-() const
	{
		return (*this) * -1.0f;
	}

	float Vector3::operator*(const Vector3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	void Vector3::operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}

	void Vector3::operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	void Vector3::operator*=(const float other)
	{
		x *= other;
		y *= other;
		z *= other;
	}

	Vector3 Vector3::Cross(const Vector3& other) const
	{
		return Vector3(
			y*other.z - z * other.y,
			z*other.x - x * other.z,
			x*other.y - y * other.x
		);
	}

	float Vector3::Dot(const Vector3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	float Vector3::Magnitude() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	float Vector3::SquareMagnitude() const
	{
		return x*x + y*y + z*z;
	}

	void Vector3::Normalize()
	{
		float mag = Magnitude();

		if (mag == 0)
			return;

		float inv = 1.0f / mag;
		x *= inv;
		y *= inv;
		z *= inv;
	}

	Vector3 Vector3::Normalized() const
	{
		float mag = Magnitude();

		if (mag == 0)
			return Vector3();

		float inv = 1.0f / mag;
		return Vector3(x * inv, y * inv, z * inv);
	}

	bool Vector3::operator==(const Vector3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool Vector3::operator!=(const Vector3& other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}

	void Vector3::Clear()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
}