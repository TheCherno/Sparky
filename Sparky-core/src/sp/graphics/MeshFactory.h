#pragma once

#include <sp/Types.h>
#include <sp/maths/maths.h>

#include <sp/graphics/Mesh.h>
#include <sp/graphics/Material.h>
#include <sp/graphics/buffers/VertexArray.h>

namespace sp { namespace graphics { namespace MeshFactory {

	//
	// TODO: Return a Mesh instead of a VertexArray
	//

	VertexArray* CreateQuad(float x, float y, float width, float height);
	VertexArray* CreateQuad(const maths::vec2& position, const maths::vec2& size);

	Mesh* CreateCube(float size, MaterialInstance* material);

} } }