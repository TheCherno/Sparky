#include "MeshFactory.h"

#include <sp/graphics/BatchRenderer2D.h>

#include <sp/sparkygl.h>
#include <sp/graphics/Renderable2D.h>
#include <sp/graphics/shaders/Shader.h>

#define SPARKY_VERTEX_ARRAYS 1

namespace sp { namespace graphics { namespace MeshFactory {

	VertexArray* CreateQuad(float x, float y, float width, float height)
	{
		using namespace maths;

		VertexData data[4];

		data[0].vertex = maths::vec3(x, y, 0);
		data[0].uv = maths::vec2(0, 1);

		data[1].vertex = maths::vec3(x, y + height, 0);
		data[1].uv = maths::vec2(0, 0);

		data[2].vertex = maths::vec3(x + width, y + height, 0);
		data[2].uv = maths::vec2(1, 0);

		data[3].vertex = maths::vec3(x + width, y, 0);
		data[3].uv = maths::vec2(1, 1);

#if SPARKY_VERTEX_ARRAYS
		API::Buffer* buffer = new API::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buffer->Bind();
		buffer->SetData(RENDERER_VERTEX_SIZE * 4, data);

		buffer->layout.Push<vec3>("position");
		buffer->layout.Push<vec2>("uv");
		buffer->layout.Push<vec2>("mask_uv");
		buffer->layout.Push<float>("tid");
		buffer->layout.Push<float>("mid");
		buffer->layout.Push<byte>("color", 4, true);

		VertexArray* result = new VertexArray();
		result->Bind();
		result->PushBuffer(buffer);
#else
#error Sparky non-vertex arrays are not yet implemented!
#endif
		return result;
	}

	VertexArray* CreateQuad(const maths::vec2& position, const maths::vec2& size)
	{
		return CreateQuad(position.x, position.y, size.x, size.y);
	}

} } }