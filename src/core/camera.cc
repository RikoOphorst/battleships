#include "camera.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Camera::Camera(float fov, float aspect, float near_z, float far_z) :
		fov_(fov),
		aspect_(aspect),
		near_z_(near_z),
		far_z_(far_z),
		width_(0),
		height_(0),
		view_(Matrix44::Identity()),
		projection_(Matrix44::Identity()),
		proj_mode_(Camera::CAMERA_PROJECTION_PERSPECTIVE)
	{
		SetTranslation(Vector3(0, 0, 0));
		SetRotation(Vector3(0, 0, 0));
		SetUp(Vector3(0, 1, 0));

		CalcAllMatrices();
	}

	//------------------------------------------------------------------------------------------------------
	Camera::~Camera()
	{

	}

	//------------------------------------------------------------------------------------------------------
	const Camera::CAMERA_PROJECTION_MODE& Camera::GetProjectionMode()
	{
		return proj_mode_;
	}

	//------------------------------------------------------------------------------------------------------
	const Matrix44& Camera::GetView() const
	{
		return view_;
	}

	//------------------------------------------------------------------------------------------------------
	const Matrix44& Camera::GetProjection() const
	{
		//return Matrix44::Perspective(120, 1280.0f / 720.0f, 0.0f, 1000.0f);
		return projection_;
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 Camera::GetTranslation() const
	{
		return translation_;
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 Camera::GetRotation() const
	{
		return rotation_;
	}

	//------------------------------------------------------------------------------------------------------
	Vector3 Camera::GetUp() const
	{
		return up_;
	}

	//------------------------------------------------------------------------------------------------------
	float Camera::GetWidth() const
	{
		return width_;
	}

	//------------------------------------------------------------------------------------------------------
	float Camera::GetHeight() const
	{
		return height_;
	}

	//------------------------------------------------------------------------------------------------------
	float Camera::GetFov() const
	{
		return fov_;
	}

	//------------------------------------------------------------------------------------------------------
	float Camera::GetAspect() const
	{
		return aspect_;
	}

	//------------------------------------------------------------------------------------------------------
	float Camera::GetNearZ() const
	{
		return near_z_;
	}

	//------------------------------------------------------------------------------------------------------
	float Camera::GetFarZ() const
	{
		return far_z_;
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetProjectionMode(const CAMERA_PROJECTION_MODE& projection_mode)
	{
		proj_mode_ = projection_mode;
		CalcProjection();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetTranslation(float x, float y, float z)
	{
		translation_.x = x;
		translation_.y = y;
		translation_.z = z;

		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetTranslation(const Vector3& translation)
	{
		translation_ = translation;
		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetRotation(float x, float y, float z)
	{
		rotation_.x = x;
		rotation_.y = y;
		rotation_.z = z;

		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetRotation(const Vector3& rotation)
	{
		rotation_ = rotation;
		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetUp(float x, float y, float z)
	{
		up_.x = x;
		up_.y = y;
		up_.z = z;

		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetUp(const Vector3& up)
	{
		up_ = up;
		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::TranslateBy(float x, float y, float z)
	{
		move_left_right_ += x;
		move_up_down_ += y;
		move_back_forward_ += z;
		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::TranslateBy(const Vector3& translation)
	{
		move_left_right_ += translation.x;
		move_up_down_ += translation.y;
		move_back_forward_ += translation.z; 
		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::RotateBy(float x, float y, float z)
	{
		rotation_.x += x;
		rotation_.y += y;
		rotation_.z += z;
		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::RotateBy(const Vector3& rotation)
	{
		rotation_ += rotation;
		CalcView();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetWidth(float width)
	{
		width_ = width;
		CalcProjection();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetHeight(float height)
	{
		height_ = height;
		CalcProjection();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetFov(float fov)
	{
		fov_ = fov;
		CalcProjection();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetAspect(float aspect)
	{
		aspect_ = aspect;
		CalcProjection();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetNearZ(float near_z)
	{
		near_z_ = near_z;
		CalcProjection();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::SetFarZ(float far_z)
	{
		far_z = far_z_;
		CalcProjection();
	}

	//------------------------------------------------------------------------------------------------------
	void Camera::CalcView()
	{
		Matrix44 rot = Matrix44::RotationFromVector(rotation_);

		target_ = rot * cam_forward_;
		target_ = target_.Normalized();

		right_ = rot * cam_right_;
		forward_ = rot * cam_forward_;
		up_ = forward_.Cross(right_);

		translation_ += move_left_right_ * right_;
		translation_ += move_back_forward_ * forward_;
		translation_ += move_up_down_ * up_;

		target_ = translation_ + target_;
		view_ = Matrix44::LookAt(translation_, target_, up_);

		move_left_right_ = 0.0f;
		move_back_forward_ = 0.0f;
		move_up_down_ = 0.0f;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Camera::CalcProjection()
	{
		switch (proj_mode_)
		{
			case CAMERA_PROJECTION_PERSPECTIVE:
				projection_ = Matrix44::Perspective(fov_, aspect_, near_z_, far_z_);
				break;
			case CAMERA_PROJECTION_ORTHOGRAPHIC:
				projection_ = Matrix44::Orthographic(width_, height_, near_z_, far_z_);
				break;
			default:
				break;
		}
	}
	
	//------------------------------------------------------------------------------------------------------
	void Camera::CalcAllMatrices()
	{
		CalcProjection();
		CalcView();
	}
}