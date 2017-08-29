#pragma once

#include "Vector3.h"

namespace igad
{
	/**
	* @struct igad::Matrix44
	* @author Riko Ophorst
	* @brief A 4x4, row-major matrix.
	* @note Treats vectors as row-vectors (1x4) with w-component 1.0f
	*/
	struct Matrix44
	{
		/**
		* @union igad::Matrix44
		* @author Riko Ophorst
		* @brief Unnamed union used for accessing the matrix's components
		* @note This is a technique that exploits the way unions work, which
		*		technically, following the C standard, would cause undefined
		*		behaviour because of the way that we use the union. However,
		*		this is fine. We use it as a way of accessing the same data
		*		with different types associated with it (1d vs 2d array).
		*/
		union
		{
			float m[4][4]; //<! 2D array of the components in the 4x4 matrix
			float f[16]; //<! 1D array of the components in the 4x4 matrix
		};

		/**
		* @brief Constructs a 4x4 matrix with the given components
		* @param[in] m00 (float) Component 0x0
		* @param[in] m01 (float) Component 0x1
		* @param[in] m02 (float) Component 0x2
		* @param[in] m03 (float) Component 0x3
		* @param[in] m10 (float) Component 1x0
		* @param[in] m11 (float) Component 1x1
		* @param[in] m12 (float) Component 1x2
		* @param[in] m13 (float) Component 1x3
		* @param[in] m20 (float) Component 2x0
		* @param[in] m21 (float) Component 2x1
		* @param[in] m22 (float) Component 2x2
		* @param[in] m23 (float) Component 2x3
		* @param[in] m30 (float) Component 3x0
		* @param[in] m31 (float) Component 3x1
		* @param[in] m32 (float) Component 3x2
		* @param[in] m33 (float) Component 3x3
		*/
		Matrix44(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		);

		/**
		* @brief Constructs a zero-matrix - all the components of the matrix are zero
		*/
		Matrix44();

		/**
		* @brief Multiplies (transforms) given row-vector (1x4 vector) with this matrix
		* @param[in] vec (const Vector3&) the vector to be multiplied (transformed)
		*/
		Vector3 operator*(const Vector3& vec) const;

		Vector3 MulDirectionVector(const Vector3& vec) const;

		/**
		* @brief Adds two matrices
		* @param[in] mat (const Matrix44&) the matrix to be added
		*/
		Matrix44 operator+(const Matrix44& mat) const;

		/**
		* @brief Subtracts two matrices
		* @param[in] mat (const Matrix44&) the matrix to be subtracted
		*/
		Matrix44 operator-(const Matrix44& mat) const;

		/**
		* @brief Multiplies two matrices
		* @param[in] mat (const Matrix44&) the matrix to be multiplied
		*/
		Matrix44 operator*(const Matrix44& mat) const;

		/**
		* @brief Compares two matrices to decide if they are equal
		* @param[in] mat (const Matrix44&) the matrix to be compared
		*/
		bool operator==(const Matrix44& mat) const;

		/**
		* @brief Get the translation (shear, technically) from the matrix
		* @note Stored in the bottom row: p(m30, m31, m32, m33)
		*/
		Vector3 GetTranslation() const;

		/**
		* @brief Sets the translation (shear, technically) in the matrix
		* @param[in] vec (const Vector3&) the vector which represents translation (shear)
		*/
		void SetTranslation(const Vector3& vec);

		/**
		* @brief Returns the X basis vector, stored in the first row: i(m00, m01, m02, m03)
		*/
		Vector3 GetXAxis() const;

		/**
		* @brief Returns the Y basis vector, stored in the second row: j(m10, m11, m12, m13)
		*/
		Vector3 GetYAxis() const;

		/**
		* @brief Returns the Z basis vector, stored in the third row: k(m20, m21, m22, m23)
		*/
		Vector3 GetZAxis() const;

		/**
		* @brief Calculates the determinant of this matrix
		* @note The determinant of a matrix is equal to its area
		*/
		float Determinant() const;

		/**
		* @brief Calculates the inverted matrix of this one
		* @note This means that if you multiply the result of this function with this matrix,
		*		you get an identity matrix. i.e.: A * A' = I
		*/
		bool Invert();

		/**
		* @brief Transposes the matrix, converting it from row-major to column major & vice versa
		*/
		void Transpose();

		/**
		* @brief Sets the orientation vectors (basis vectors: i, j, k) of the matrix
		* @param[in] x (const Vector3&) The new i vector of the matrix
		* @param[in] y (const Vector3&) The new j vector of the matrix
		* @param[in] z (const Vector3&) The new k vector of the matrix
		* @note THIS OVERRIDES ANY SCALING THAT WAS PREVIOUSLY STORED IN THIS MATRIX!
		*/
		void SetOrientation(
			const Vector3& x,
			const Vector3& y,
			const Vector3& z
		);

		/**
		* @brief Sets the yaw, pitch & roll of the matrix
		* @param[in] x (float) The rotation in radians around the X axis
		* @param[in] y (float) The rotation in radians around the Y axis
		* @param[in] z (float) The rotation in radians around the Z axis
		*/
		void SetEulerAxis(
			float x, 
			float y, 
			float z
		);

		/**
		* @brief Generates a 4x4 identity matrix
		*/
		static Matrix44 Identity();

		/**
		* @brief Creates a translation (shearing, technically) matrix
		* @param[in] x (float) the x translation of the matrix
		* @param[in] y (float) the y translation of the matrix
		* @param[in] z (float) the z translation of the matrix
		*/
		static Matrix44 Translation(
			float x, 
			float y, 
			float z
		);

		/**
		* @brief Creates a translation (shearing, technically) matrix from a vector
		* @param[in] vector (float) the translation of the matrix
		*/
		static Matrix44 TranslationFromVector(const Vector3& vector);

		/**
		* @brief Creates a scaling matrix
		* @param[in] x (float) the scaling along the x-axis
		* @param[in] y (float) the scaling along the y-axis
		* @param[in] z (float) the scaling along the z-axis
		*/
		static Matrix44 CreateScale(
			float x, 
			float y, 
			float z
		);

		/**
		* @brief Creates a scaling matrix from a vector
		* @param[in] vector (float) the scaling vector of the matrix
		*/
		static Matrix44 ScalingFromVector(const Vector3& scale);

		/**
		* @brief Creates a rotation matrix based on the given angles in radians
		* @param[in] x (float) the rotation around the x-axis in radians
		* @param[in] y (float) the rotation around the y-axis in radians
		* @param[in] z (float) the rotation around the z-axis in radians
		*/
		static Matrix44 Rotation(
			float x, 
			float y, 
			float z
		);

		/**
		* @brief Creates a rotation matrix based on the given angles in radians inside a vector
		* @param[in] rotation (const Vector3&) the rotation of the matrix along each axis in radians
		*/
		static Matrix44 RotationFromVector(const Vector3& rotation);

		/**
		* @brief Creates a rotation matrix that around a given arbitrary axis in radians
		* @param[in] angle (float) the angle in radians that should be rotated around the axis
		* @param[in] axis (const Vector3&) the axis the matrix should rotate around
		*/
		static Matrix44 RotateArbitraryAxis(float angle, const Vector3& axis);

		/**
		* @brief Creates a rotation matrix that rotates around the X axis
		* @param[in] angle (float) the angle in radians that should be rotated around the x-axis
		*/
		static Matrix44 RotateX(float angle);

		/**
		* @brief Creates a rotation matrix that rotates around the Y axis
		* @param[in] angle (float) the angle in radians that should be rotated around the y-axis
		*/
		static Matrix44 RotateY(float angle);

		/**
		* @brief Creates a rotation matrix that rotates around the z axis
		* @param[in] angle (float) the angle in radians that should be rotated around the z-axis
		*/
		static Matrix44 RotateZ(float angle);

		/**
		* @brief Creates a orthographic view frustum matrix
		* @param[in] width (float) the width of the planes
		* @param[in] height (float) the height of the planes
		* @param[in] near_z (float) the z depth of the near plane
		* @param[in] far_z (float) the z depth of the far plane
		* @param[in] left_handed (bool) should the frustum be left handed?
		*/
		static Matrix44 Orthographic(float width, float height, float near_z, float far_z, bool left_handed = true);

		/**
		* @brief Creates a perspective view frustum matrix
		* @param[in] fov (float) the fov of the frustum
		* @param[in] aspect (float) the aspect ratio of the near plane
		* @param[in] near_z (float) the z depth of the near plane
		* @param[in] far_z (float) the z depth of the far plane
		* @param[in] left_handed (bool) should the frustum be left handed?
		*/
		static Matrix44 Perspective(float fov, float aspect, float near_z, float far_z, bool left_handed = true);

		/**
		* @brief Creates a look-at matrix
		* @param[in] eye (const Vector3&) the position of the eye - where we are look from
		* @param[in] focus (const Vector3&) the position on which the eye should be focused
		* @param[in] up (const Vector3&) the direction that is up (usually v(0, 1, 0))
		* @param[in] left_handed (bool) should the frustum be left handed?
		*/
		static Matrix44 LookAt(const Vector3& eye, const Vector3& focus, const Vector3& up, bool left_handed = true);
	};
}