#pragma once

#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/maths/maths.h"

#include "sp/graphics/Mesh.h"
#include "sp/graphics/Material.h"
#include "sp/graphics/buffers/VertexArray.h"

namespace sp { namespace graphics { namespace MeshFactory {

	//
	// TODO: Return a Mesh instead of a VertexArray
	//

	SP_API VertexArray* CreateQuad(float x, float y, float width, float height);
	SP_API VertexArray* CreateQuad(const maths::vec2& position, const maths::vec2& size);

	SP_API Mesh* CreateCube(float size, MaterialInstance* material);
	SP_API Mesh* CreatePlane(float width, float height, const maths::vec3& normal, MaterialInstance* material);

} } }