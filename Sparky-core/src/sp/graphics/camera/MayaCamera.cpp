#include "sp/sp.h"
#include "MayaCamera.h"

#include "../Window.h"

namespace sp { namespace graphics {

	using namespace maths;

	MayaCamera::MayaCamera(const maths::mat4& projectionMatrix)
		: Camera(projectionMatrix)
	{
		// Sensible defaults
		m_PanSpeed = 0.0015f;
		m_RotationSpeed = 0.002f;
		m_ZoomSpeed = 0.2f;

		m_Position = vec3(0.0f, 25.0f, -25.0f);
		m_Rotation = vec3(90.0f, 0.0f, 0.0f);

		m_FocalPoint = vec3::Zero();
		m_Distance = m_Position.Distance(m_FocalPoint);

		m_Yaw = 3.0f * M_PI / 4.0f;
		m_Pitch = M_PI / 4.0f;
	}

	void MayaCamera::Update()
	{
		Window* window = Window::GetWindowClass(nullptr);

		if (window->IsKeyPressed(VK_ALT))
		{
			const vec2& mouse = window->GetMousePosition();
			vec2 delta = mouse - m_InitialMousePosition;
			m_InitialMousePosition = mouse;

			if (window->IsMouseButtonPressed(SP_MOUSE_MIDDLE))
				MousePan(delta);
			else if (window->IsMouseButtonPressed(SP_MOUSE_LEFT))
				MouseRotate(delta);
			else if (window->IsMouseButtonPressed(SP_MOUSE_RIGHT))
				MouseZoom(delta.y);

		}

		// MouseZoom(window->GetMouseScrollPosition().y);

		m_ViewMatrix = mat4::Rotate(GetOrientation().Conjugate());
		m_ViewMatrix *= mat4::Translate(-GetPosition());
	}

	void MayaCamera::MousePan(const maths::vec2& delta)
	{
		m_FocalPoint += -GetRightDirection() * delta.x * m_PanSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * m_PanSpeed * m_Distance;
	}

	void MayaCamera::MouseRotate(const maths::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * m_RotationSpeed;
		m_Pitch += delta.y * m_RotationSpeed;
	}

	void MayaCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * m_ZoomSpeed;
	}

	vec3 MayaCamera::GetUpDirection()
	{
		return Quaternion::Rotate(GetOrientation(), vec3::YAxis());
	}

	vec3 MayaCamera::GetRightDirection()
	{
		return Quaternion::Rotate(GetOrientation(), vec3::XAxis());
	}

	vec3 MayaCamera::GetForwardDirection()
	{
		return Quaternion::Rotate(GetOrientation(), -vec3::ZAxis());
	}

	vec3 MayaCamera::GetPosition()
	{
		return m_FocalPoint - m_Distance * GetForwardDirection();
	}

	Quaternion MayaCamera::GetOrientation()
	{
		return Quaternion::RotationY(-m_Yaw) * Quaternion::RotationX(-m_Pitch);
	}

} }