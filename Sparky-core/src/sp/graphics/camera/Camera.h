#pragma once

#include "sp/Common.h"
#include "sp/maths/maths.h"

namespace sp { namespace graphics {

	class SP_API Camera
	{
	protected:
		maths::mat4 m_ProjectionMatrix, m_ViewMatrix;
		maths::vec3 m_Position, m_Rotation, m_FocalPoint;
	public:
		Camera(const maths::mat4& projectionMatrix);

		virtual void Focus() { }
		virtual void Update() { }

		inline const maths::vec3& GetPosition() const { return m_Position; }
		inline void SetPosition(const maths::vec3& position) { m_Position = position; }

		inline const maths::vec3& GetRotation() const { return m_Rotation; }
		inline void SetRotation(const maths::vec3& rotation) { m_Rotation = rotation; }

		inline const maths::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline void SetProjectionMatrix(const maths::mat4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }

		inline void Translate(const maths::vec3& translation) { m_Position += translation; }
		inline void Rotate(const maths::vec3& rotation) { m_Rotation += rotation; }

		inline void Translate(float x, float y, float z) { m_Position += maths::vec3(x, y, z); }
		inline void Rotate(float x, float y, float z) { m_Rotation += maths::vec3(x, y, z); }

		inline const maths::vec3& GetFocalPoint() const { return m_FocalPoint; }

		inline const maths::mat4& GetViewMatrix() { return m_ViewMatrix; }

	};

} }