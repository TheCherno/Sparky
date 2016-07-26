#include "sp/sp.h"
#include "FPSCamera.h"

#include "sp/app/Input.h"
#include "sp/utils/Log.h"

#include "sp/app/Application.h"
#include "sp/debug/DebugMenu.h"

namespace sp { namespace graphics {

	using namespace maths;

	FPSCamera::FPSCamera(const maths::mat4& projectionMatrix)
		: Camera(projectionMatrix), m_MouseSensitivity(0.002f), m_Speed(0.4f), m_SprintSpeed(m_Speed * 4.0f), m_MouseWasGrabbed(false)
	{
		m_Position = vec3(0.0f, 25.0f, -25.0f);
		m_Rotation = vec3(90.0f, 0.0f, 0.0f);
		m_Yaw = 2.4f;
		m_Pitch = 0.7f;

		debug::DebugMenu::Add("Camera/FPS Camera Speed", &m_Speed);
		debug::DebugMenu::Add("Camera/FPS Camera Sprint Speed", &m_SprintSpeed);
	}

	FPSCamera::~FPSCamera()
	{
		debug::DebugMenu::Remove("Camera/FPS Camera Speed");
		debug::DebugMenu::Remove("Camera/FPS Camera Sprint Speed");
	}

	void FPSCamera::Focus()
	{
		Input::GetInputManager()->SetMouseCursor(SP_NO_CURSOR);
	}

	void FPSCamera::Update()
	{
		vec2 windowSize = Application::GetApplication().GetWindowSize();
		vec2 windowCenter = vec2((float)(int32)(windowSize.x / 2.0f), (float)(int32)(windowSize.y / 2.0f));

		if (Input::IsMouseButtonPressed(SP_MOUSE_RIGHT))
		{
			if (!Input::GetInputManager()->IsMouseGrabbed())
			{
				Input::GetInputManager()->SetMouseGrabbed(true);
				Input::GetInputManager()->SetMouseCursor(SP_NO_CURSOR);
			}
		}

		if (Input::GetInputManager()->IsMouseGrabbed())
		{
			vec2 mouse = Input::GetInputManager()->GetMousePosition();
			mouse.x -= windowCenter.x;
			mouse.y -= windowCenter.y;
			if (m_MouseWasGrabbed)
			{
				m_Yaw += mouse.x * m_MouseSensitivity;
				m_Pitch += mouse.y * m_MouseSensitivity;
			}
			m_MouseWasGrabbed = true;
			Input::GetInputManager()->SetMousePosition(windowCenter);

			Quaternion orientation = GetOrientation();
			m_Rotation = orientation.ToEulerAngles() * (180.0f / SP_PI);

			vec3 forward = GetForwardDirection(orientation);
			vec3 right = GetRightDirection(orientation);
			vec3 up = vec3::YAxis();
			float speed = Input::IsKeyPressed(SP_KEY_SHIFT) ? m_SprintSpeed : m_Speed;
			if (Input::IsKeyPressed(SP_KEY_W))
				m_Position += forward * speed;
			else if (Input::IsKeyPressed(SP_KEY_S))
				m_Position -= forward * speed;

			if (Input::IsKeyPressed(SP_KEY_A))
				m_Position -= right * speed;
			else if (Input::IsKeyPressed(SP_KEY_D))
				m_Position += right * speed;

			if (Input::IsKeyPressed(SP_KEY_SPACE))
				m_Position += up * speed;
			if (Input::IsKeyPressed(SP_KEY_CONTROL))
				m_Position -= up * speed;

			mat4 rotation = mat4::Rotate(orientation.Conjugate());
			mat4 translation = mat4::Translate(-m_Position);
			m_ViewMatrix = rotation * translation;
		}

		if (Input::IsKeyPressed(SP_KEY_ESCAPE))
		{
			Input::GetInputManager()->SetMouseGrabbed(false);
			Input::GetInputManager()->SetMouseCursor(1);
			m_MouseWasGrabbed = false;
		}
	}

	Quaternion FPSCamera::GetOrientation() const
	{
		return Quaternion::RotationY(-m_Yaw) * Quaternion::RotationX(-m_Pitch);
	}

	vec3 FPSCamera::GetForwardDirection(const Quaternion& orientation) const
	{
		return Quaternion::Rotate(orientation, -vec3::ZAxis());
	}

	vec3 FPSCamera::GetUpDirection(const Quaternion& orientation) const
	{
		return Quaternion::Rotate(orientation, vec3::YAxis());
	}

	vec3 FPSCamera::GetRightDirection(const Quaternion& orientation) const
	{
		return Quaternion::Rotate(orientation, vec3::XAxis());
	}

} }