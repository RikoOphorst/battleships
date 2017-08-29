#pragma once

#include "../math/matrix44.h"
#include "../math/vector3.h"

namespace igad
{
	class Camera
	{
	public:
		enum CAMERA_PROJECTION_MODE
		{
			CAMERA_PROJECTION_ORTHOGRAPHIC,
			CAMERA_PROJECTION_PERSPECTIVE
		};

		Camera(float fov, float aspect, float near_z, float far_z);
		~Camera();

		const CAMERA_PROJECTION_MODE& GetProjectionMode();

		const Matrix44& GetView() const;
		const Matrix44& GetProjection() const;

		Vector3 GetTranslation() const;
		Vector3 GetRotation() const;
		Vector3 GetUp() const;

		float GetWidth() const;
		float GetHeight() const;
		float GetFov() const;
		float GetAspect() const;

		float GetNearZ() const;
		float GetFarZ() const;

		void SetProjectionMode(const CAMERA_PROJECTION_MODE& projection_mode);

		void SetTranslation(float x, float y, float z);
		void SetTranslation(const Vector3& translation);
		void SetRotation(float x, float y, float z);
		void SetRotation(const Vector3& rotation);
		void SetUp(float x, float y, float z);
		void SetUp(const Vector3& up);
		void TranslateBy(float x, float y, float z);
		void TranslateBy(const Vector3& translation);
		void RotateBy(float x, float y, float z);
		void RotateBy(const Vector3& translation);

		void SetWidth(float width);
		void SetHeight(float height);
		void SetFov(float fov);
		void SetAspect(float aspect);

		void SetNearZ(float near_z);
		void SetFarZ(float far_z);

		void CalcView();
		void CalcProjection();
		void CalcAllMatrices();

	private:
		CAMERA_PROJECTION_MODE proj_mode_;

		Matrix44 view_;
		Matrix44 projection_;

		Vector3 translation_;
		Vector3 rotation_;
		Vector3 up_;

		Vector3 right_;
		Vector3 forward_;
		Vector3 target_;

		float move_left_right_;
		float move_back_forward_;
		float move_up_down_;

		const Vector3 cam_forward_ = Vector3(0.0f, 0.0f, 1.0f);
		const Vector3 cam_right_ = Vector3(1.0f, 0.0f, 0.0f);

		float width_;
		float height_;
		float fov_;
		float aspect_;

		float near_z_;
		float far_z_;
	};
}