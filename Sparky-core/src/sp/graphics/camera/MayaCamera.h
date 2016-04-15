#pragma once

#include "Camera.h"
#include "sp/Common.h"

namespace sp { namespace graphics {

	class SP_API MayaCamera : public Camera
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
		void Focus() override;
		void Update() override;

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }
	private:
		void MousePan(const maths::vec2& delta);
		void MouseRotate(const maths::vec2& delta);
		void MouseZoom(float delta);

		// TODO: Move up to camera class
		maths::vec3 GetUpDirection();
		maths::vec3 GetRightDirection();
		maths::vec3 GetForwardDirection();

		maths::vec3 CalculatePosition();
		maths::Quaternion GetOrientation();
	};

} }