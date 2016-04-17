#include "sp/sp.h"
#include "Camera.h"

#include "sp/debug/DebugMenu.h"

namespace sp { namespace graphics {

	Camera::Camera(const maths::mat4& projectionMatrix)
		: m_ProjectionMatrix(projectionMatrix)
	{
		m_ViewMatrix = maths::mat4::Identity();
		m_Position = maths::vec3();
		m_Rotation = maths::vec3();
		m_Yaw = 0.0f;
		m_Pitch = 0.7f;
		m_Roll = 0.0f;
	}

} }