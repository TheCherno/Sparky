#pragma once

#include "Camera.h"

namespace sp { namespace graphics {

	class MayaCamera : public Camera
	{
	private:
		bool m_Panning, m_Rotating;
		maths::vec2 m_InitialMousePosition;
		maths::vec3 m_InitialFocalPoint, m_InitialRotation;

		float m_Distance;
		float m_PanSpeed, m_RotationSpeed, m_ZoomSpeed;

		float m_Pitch, m_Yaw;
	public:
		MayaCamera(const maths::mat4& projectionMatrix);
		void Update() override;
	private:
		void MousePan(const maths::vec2& delta);
		void MouseRotate(const maths::vec2& delta);
		void MouseZoom(float delta);

		// TODO: Move up to camera class
		maths::vec3 GetUpDirection();
		maths::vec3 GetRightDirection();
		maths::vec3 GetForwardDirection();

		maths::vec3 GetPosition();
		maths::Quaternion GetOrientation();
	};

} }