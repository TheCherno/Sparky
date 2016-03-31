#pragma once

#include "sp/Common.h"
#include "sp/maths/maths.h"
#include "sp/graphics/shaders/Shader.h"
#include "sp/graphics/Mesh.h"
#include "sp/graphics/camera/Camera.h"

namespace sp { namespace debug {

	enum class DebugRenderMeshFlags
	{
		NONE		= 0,
		WIREFRAME	= BIT(0),
		NORMALS		= BIT(1),
		BINORMALS	= BIT(2),
		TANGENTS	= BIT(3)
	};

	class DebugRenderer
	{
 	private:
		DebugRenderer() {}
	public:
		SP_DEBUG_METHOD_V(static void Init())
		SP_DEBUG_METHOD_V(static void Shutdown())
		SP_DEBUG_METHOD_V(static void DrawLine(const maths::vec3& start, const maths::vec3& end, uint color = 0xffffffff))
		SP_DEBUG_METHOD_V(static void DrawMesh(const graphics::Mesh* mesh, DebugRenderMeshFlags flags, const maths::mat4& transform = maths::mat4::Identity()))

		SP_DEBUG_METHOD_V(static void SetCamera(graphics::Camera* camera))

		SP_DEBUG_METHOD_V(static void Present())
	private:
		SP_DEBUG_METHOD_V(static void DrawLineInternal(const maths::vec3& start, const maths::vec3& end, uint color))
	};

} }