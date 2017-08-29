#include "matrix44.h"

#include <memory>
#include <cmath>

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Matrix44::Matrix44(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;
		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;
		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;
		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44::Matrix44()
	{
		for (int i = 0; i < 16; i++)
		{
			f[i] = 0.0f;
		}
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 Matrix44::operator*(const Vector3& vec) const
	{
		return Vector3(
			m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0],
			m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1],
			m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2]
			);
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 Matrix44::MulDirectionVector(const Vector3 & vec) const
	{
		return Vector3(
			m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z,
			m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z,
			m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::operator+(const Matrix44& mat) const
	{
		Matrix44 product = Matrix44();

		for (int i = 0; i < 16; i++)
		{
			product.f[i] = f[i] + mat.f[i];
		}

		return product;
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::operator-(const Matrix44& mat) const
	{
		Matrix44 product = Matrix44();

		for (int i = 0; i < 16; i++)
		{
			product.f[i] = f[i] - mat.f[i];
		}

		return product;
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::operator*(const Matrix44& mat) const
	{
		Matrix44 matrix = Matrix44();

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				for (int inner = 0; inner < 4; inner++)
				{
					matrix.m[row][col] += m[row][inner] * mat.m[inner][col];
				}
			}
		}

		return matrix;
	}

	bool Matrix44::operator==(const Matrix44& mat) const
	{
		bool equal = true;
		for (int i = 0; i < 16; i++)
		{
			if (f[i] != mat.f[i])
			{
				equal = false;
			}
		}

		return equal;
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 Matrix44::GetTranslation() const
	{
		return Vector3(m[3][0], m[3][1], m[3][2]);
	}

	//------------------------------------------------------------------------------------------------------
	void Matrix44::SetTranslation(const Vector3& vec)
	{
		m[3][0] = vec.x;
		m[3][1] = vec.y;
		m[3][2] = vec.z;
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 Matrix44::GetXAxis() const
	{
		return Vector3(m[0][0], m[0][1], m[0][2]);
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 Matrix44::GetYAxis() const
	{
		return Vector3(m[1][0], m[1][1], m[1][2]);
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 Matrix44::GetZAxis() const
	{
		return Vector3(m[2][0], m[2][1], m[2][2]);
	}

	//------------------------------------------------------------------------------------------------------
	float Matrix44::Determinant() const
	{
		return m[0][0] * (m[1][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - m[1][2] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) + m[1][3] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]))
			- m[0][1] * (m[1][0] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - m[1][2] * (m[2][0] * m[3][3] - m[3][0] * m[2][3]) + m[1][3] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]))
			+ m[0][2] * (m[1][0] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) - m[1][1] * (m[2][0] * m[3][3] - m[3][0] * m[2][3]) + m[1][3] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]))
			- m[0][3] * (m[1][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]) - m[1][1] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]) + m[1][2] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]));
	}

	//------------------------------------------------------------------------------------------------------
	bool Matrix44::Invert()
	{
		float inv[16], det;
		int i;

		inv[0] = f[5] * f[10] * f[15] -
			f[5] * f[11] * f[14] -
			f[9] * f[6] * f[15] +
			f[9] * f[7] * f[14] +
			f[13] * f[6] * f[11] -
			f[13] * f[7] * f[10];

		inv[4] = -f[4] * f[10] * f[15] +
			f[4] * f[11] * f[14] +
			f[8] * f[6] * f[15] -
			f[8] * f[7] * f[14] -
			f[12] * f[6] * f[11] +
			f[12] * f[7] * f[10];

		inv[8] = f[4] * f[9] * f[15] -
			f[4] * f[11] * f[13] -
			f[8] * f[5] * f[15] +
			f[8] * f[7] * f[13] +
			f[12] * f[5] * f[11] -
			f[12] * f[7] * f[9];

		inv[12] = -f[4] * f[9] * f[14] +
			f[4] * f[10] * f[13] +
			f[8] * f[5] * f[14] -
			f[8] * f[6] * f[13] -
			f[12] * f[5] * f[10] +
			f[12] * f[6] * f[9];

		inv[1] = -f[1] * f[10] * f[15] +
			f[1] * f[11] * f[14] +
			f[9] * f[2] * f[15] -
			f[9] * f[3] * f[14] -
			f[13] * f[2] * f[11] +
			f[13] * f[3] * f[10];

		inv[5] = f[0] * f[10] * f[15] -
			f[0] * f[11] * f[14] -
			f[8] * f[2] * f[15] +
			f[8] * f[3] * f[14] +
			f[12] * f[2] * f[11] -
			f[12] * f[3] * f[10];

		inv[9] = -f[0] * f[9] * f[15] +
			f[0] * f[11] * f[13] +
			f[8] * f[1] * f[15] -
			f[8] * f[3] * f[13] -
			f[12] * f[1] * f[11] +
			f[12] * f[3] * f[9];

		inv[13] = f[0] * f[9] * f[14] -
			f[0] * f[10] * f[13] -
			f[8] * f[1] * f[14] +
			f[8] * f[2] * f[13] +
			f[12] * f[1] * f[10] -
			f[12] * f[2] * f[9];

		inv[2] = f[1] * f[6] * f[15] -
			f[1] * f[7] * f[14] -
			f[5] * f[2] * f[15] +
			f[5] * f[3] * f[14] +
			f[13] * f[2] * f[7] -
			f[13] * f[3] * f[6];

		inv[6] = -f[0] * f[6] * f[15] +
			f[0] * f[7] * f[14] +
			f[4] * f[2] * f[15] -
			f[4] * f[3] * f[14] -
			f[12] * f[2] * f[7] +
			f[12] * f[3] * f[6];

		inv[10] = f[0] * f[5] * f[15] -
			f[0] * f[7] * f[13] -
			f[4] * f[1] * f[15] +
			f[4] * f[3] * f[13] +
			f[12] * f[1] * f[7] -
			f[12] * f[3] * f[5];

		inv[14] = -f[0] * f[5] * f[14] +
			f[0] * f[6] * f[13] +
			f[4] * f[1] * f[14] -
			f[4] * f[2] * f[13] -
			f[12] * f[1] * f[6] +
			f[12] * f[2] * f[5];

		inv[3] = -f[1] * f[6] * f[11] +
			f[1] * f[7] * f[10] +
			f[5] * f[2] * f[11] -
			f[5] * f[3] * f[10] -
			f[9] * f[2] * f[7] +
			f[9] * f[3] * f[6];

		inv[7] = f[0] * f[6] * f[11] -
			f[0] * f[7] * f[10] -
			f[4] * f[2] * f[11] +
			f[4] * f[3] * f[10] +
			f[8] * f[2] * f[7] -
			f[8] * f[3] * f[6];

		inv[11] = -f[0] * f[5] * f[11] +
			f[0] * f[7] * f[9] +
			f[4] * f[1] * f[11] -
			f[4] * f[3] * f[9] -
			f[8] * f[1] * f[7] +
			f[8] * f[3] * f[5];

		inv[15] = f[0] * f[5] * f[10] -
			f[0] * f[6] * f[9] -
			f[4] * f[1] * f[10] +
			f[4] * f[2] * f[9] +
			f[8] * f[1] * f[6] -
			f[8] * f[2] * f[5];

		det = f[0] * inv[0] + f[1] * inv[4] + f[2] * inv[8] + f[3] * inv[12];

		if (det == 0)
			return false;

		det = 1.0f / det;

		for (i = 0; i < 16; i++)
			f[i] = inv[i] * det;
	}

	//------------------------------------------------------------------------------------------------------
	void Matrix44::Transpose()
	{
		float new_m[4][4];
		int row = 0;
		int col = 0;

		for (row = 0; row < 4; row++)
		{
			for (col = 0; col < 4; col++)
			{
				new_m[col][row] = m[row][col];
			}
		}

		memcpy(&m, &new_m, sizeof(float) * 16);
	}

	//------------------------------------------------------------------------------------------------------
	void Matrix44::SetOrientation(const Vector3& x, const Vector3& y, const Vector3& z)
	{
		m[0][0] = x.x;
		m[0][1] = x.y;
		m[0][2] = x.z;

		m[1][0] = y.x;
		m[1][1] = y.y;
		m[1][2] = y.z;

		m[2][0] = z.x;
		m[2][1] = z.y;
		m[2][2] = z.z;
	}

	//------------------------------------------------------------------------------------------------------
	void Matrix44::SetEulerAxis(float x, float y, float z)
	{
		float cx = cos(x); // yaw
		float sx = sin(x);
		float cy = cos(y); // p
		float sy = sin(y);
		float cz = cos(z); // r
		float sz = sin(z);

		Matrix44 product = Matrix44(
			cy*cz + sz*sy*sx, cx*sz, sz*sx*cy - sy*cz, 0,
			sy*sx*cz - sz*cy, cz*cx, sy*sz + cz*cy*sx, 0,
			sy*cx, -sx, cy*cx, 0,
			0, 0, 0, 1
		);

		*f = *product.f;
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::Identity()
	{
		return Matrix44(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::Translation(float x, float y, float z)
	{
		return Matrix44(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::TranslationFromVector(const Vector3& vector)
	{
		return Matrix44(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0, 
			vector.x, vector.y, vector.z, 1
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::CreateScale(float x, float y, float z)
	{
		return Matrix44(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::ScalingFromVector(const Vector3& scale)
	{
		return Matrix44(
			scale.x, 0, 0, 0,
			0, scale.y, 0, 0,
			0, 0, scale.z, 0,
			0, 0, 0, 1
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::Rotation(float x, float y, float z)
	{
		float cx = cos(x); // yaw
		float sx = sin(x);
		float cy = cos(y); // p
		float sy = sin(y);
		float cz = cos(z); // r
		float sz = sin(z);

		return Matrix44(
			cy*cz + sz*sy*sx, cx*sz, sz*sx*cy - sy*cz, 0,
			sy*sx*cz - sz*cy, cz*cx, sy*sz + cz*cy*sx, 0,
			sy*cx, -sx, cy*cx, 0,
			0, 0, 0, 1
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::RotationFromVector(const Vector3& rotation)
	{
		float cx = cos(rotation.x); // yaw
		float sx = sin(rotation.x);
		float cy = cos(rotation.y); // p
		float sy = sin(rotation.y);
		float cz = cos(rotation.z); // r
		float sz = sin(rotation.z);

		return Matrix44(
			cy*cz + sz*sy*sx, cx*sz, sz*sx*cy - sy*cz, 0,
			sy*sx*cz - sz*cy, cz*cx, sy*sz + cz*cy*sx, 0,
			sy*cx, -sx, cy*cx, 0,
			0, 0, 0, 1
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::RotateArbitraryAxis(float angle, const Vector3& axis)
	{
		Matrix44 rotate = Matrix44::Identity();

		float	u = axis.x, 
				v = axis.y, 
				w = axis.z;
		
		float cosa = cos(angle);
		float sina = sin(angle);

		rotate.m[0][0] = (1 - cosa) * u * u + cosa;
		rotate.m[0][1] = u * v * (1 - cosa) - w * sina;
		rotate.m[0][2] = u * w * (1 - cosa) + v * sina;
		rotate.m[0][3] = 0;

		rotate.m[1][0] = u * v * (1 - cosa) + w * sina;
		rotate.m[1][1] = ((1 - cosa) * v * v + cosa);
		rotate.m[1][2] = v * w * (1 - cosa) - u * sina;
		rotate.m[1][3] = 0;

		rotate.m[2][0] = u * w * (1 - cosa) - v * sina;
		rotate.m[2][1] = v * w * (1 - cosa) + u * sina;
		rotate.m[2][2] = ((1 - cosa) * w * w + cosa);
		rotate.m[2][3] = 0;

		rotate.Transpose();

		return rotate;
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::RotateX(float angle)
	{
		return Matrix44(
			1, 0, 0, 0,
			0, cos(angle), sin(angle), 0,
			0, -sin(angle), cos(angle), 0,
			0, 0, 0, 1
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::RotateY(float angle)
	{
		return Matrix44(
			cos(angle), 0, -sin(angle), 0,
			0, 1, 0, 0,
			sin(angle), 0, cos(angle), 0,
			0, 0, 0, 1
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::RotateZ(float angle)
	{
		return Matrix44(
			cos(angle), sin(angle), 0, 0,
			-sin(angle), cos(angle), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::Orthographic(float width, float height, float near_z, float far_z, bool left_handed)
	{
		if (left_handed == true)
		{
			return Matrix44(
				2 / width, 0, 0, 0,
				0, 2 / height, 0, 0,
				0, 0, 1 / (far_z - near_z), 0,
				0, 0, -near_z / (far_z - near_z), 1
			);
		}
		else
		{
			return Matrix44(
				2 / width, 0, 0, 0,
				0, 2 / height, 0, 0,
				0, 0, 1 / (near_z - far_z), 0,
				0, 0, near_z / (near_z - far_z), 1
			);
		}
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::Perspective(float fov, float aspect, float near_z, float far_z, bool left_handed)
	{
		if (fov <= 0 || aspect == 0)
		{
			return Matrix44();
		}

		float y_scale = 1 / tan(fov / 2);
		float x_scale = y_scale / aspect;

		if (left_handed)
		{
			return Matrix44(
				x_scale, 0, 0, 0,
				0, y_scale, 0, 0,
				0, 0, far_z / (far_z - near_z), 1,
				0, 0, -near_z * far_z / (far_z - near_z), 0
			);
		}
		else
		{
			return Matrix44(
				x_scale, 0, 0, 0,
				0, y_scale, 0, 0,
				0, 0, far_z / (near_z - far_z), -1,
				0, 0, near_z * far_z / (near_z - far_z), 0
			);
		}
	}

	//------------------------------------------------------------------------------------------------------
	Matrix44 Matrix44::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up, bool left_handed)
	{
		if (left_handed)
		{
			Vector3 z = (center - eye).Normalized();
			Vector3 x = up.Cross(z).Normalized();
			Vector3 y = z.Cross(x);

			return Matrix44(
				x.x,			y.x,			z.x, 0,
				x.y,			y.y,			z.y, 0,
				x.z,			y.z,			z.z, 0,
				-x.Dot(eye), -y.Dot(eye), -z.Dot(eye),	1
			);
		}
		else
		{
			Vector3 z = (eye - center).Normalized();
			Vector3 x = up.Cross(z).Normalized();
			Vector3 y = z.Cross(x);

			return Matrix44(
				x.x,		y.x,		z.x,		0,
				x.y,		y.y,		z.y,		0,
				x.z,		y.z,		z.z,		0,
				x.Dot(eye), y.Dot(eye), z.Dot(eye), 1
			);
		}
	}
}