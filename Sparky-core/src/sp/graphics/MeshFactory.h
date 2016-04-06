#pragma once

#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/maths/maths.h"

#include "sp/graphics/Mesh.h"
#include "sp/graphics/Material.h"
#include "sp/graphics/API/VertexArray.h"

namespace sp { namespace graphics { namespace MeshFactory {

	//
	// TODO: Return a Mesh instead of a VertexArray
	//

	SP_API Mesh* CreateQuad(float x, float y, float width, float height, MaterialInstance* material);
	SP_API Mesh* CreateQuad(const maths::vec2& position, const maths::vec2& size, MaterialInstance* material);

	SP_API Mesh* CreateCube(float size, MaterialInstance* material);
	SP_API Mesh* CreatePlane(float width, float height, const maths::vec3& normal, MaterialInstance* material);

} } }