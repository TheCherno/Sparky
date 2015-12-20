#include "sp/sp.h"
#include "Camera.h"

namespace sp { namespace graphics {

	Camera::Camera(const maths::mat4& projectionMatrix)
		: m_ProjectionMatrix(projectionMatrix)
	{
		m_ViewMatrix = maths::mat4::Identity();
		m_Position = maths::vec3();
		m_Rotation = maths::vec3();
	}

} }