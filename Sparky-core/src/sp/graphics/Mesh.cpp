#include "sp/sp.h"
#include "Mesh.h"

#include "Renderer3D.h"

#include <GL/glew.h>

#include "sp/debug/DebugRenderer.h"

namespace sp { namespace graphics {

	Mesh::Mesh(API::VertexArray* vertexArray, API::IndexBuffer* indexBuffer, MaterialInstance* materialInstance)
		: m_VertexArray(vertexArray), m_IndexBuffer(indexBuffer), m_MaterialInstance(materialInstance), m_AABB()
	{
#ifdef SP_DEBUG
		m_DebugDraw = true;
#endif
		ComputeAABB();
	}

	Mesh::Mesh(const Mesh* mesh)
		: m_VertexArray(mesh->m_VertexArray), m_IndexBuffer(mesh->m_IndexBuffer), m_MaterialInstance(mesh->m_MaterialInstance), m_AABB(mesh->GetAABB())
	{
#ifdef SP_DEBUG
		m_DebugDraw = mesh->m_DebugDraw;
#endif
	}

	Mesh::~Mesh()
	{
		spdel m_VertexArray;
		spdel m_IndexBuffer;
		spdel m_MaterialInstance;
	}

	void Mesh::ComputeAABB()
	{
		using namespace maths;

		API::VertexBuffer* buffer = m_VertexArray->GetBuffer();
		byte* vertices = (byte*)buffer->GetData();
		uint offset = buffer->GetLayout().GetElement("POSITION")->offset;
		vec3& min = m_AABB.min;
		vec3& max = m_AABB.max;
		for (uint i = 0; i < buffer->GetSize(); i += buffer->GetLayout().GetStride())
		{
			vec3& pos = *(vec3*)&vertices[i + offset];
			if (i == 0)
			{
				min = pos;
				max = pos;
			}
			else
			{
				min = vec3(std::min(min.x, pos.x), std::min(min.y, pos.y), std::min(min.z, pos.z));
				max = vec3(std::max(max.x, pos.x), std::max(max.y, pos.y), std::max(max.z, pos.z));
			}
		}
	}

	void Mesh::Render(Renderer3D& renderer)
	{
		m_MaterialInstance->Bind();

		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
		m_VertexArray->Draw(m_IndexBuffer->GetCount());
		m_IndexBuffer->Unbind();
		m_VertexArray->Unbind();

		m_MaterialInstance->Unbind();
	}

#ifdef SP_DEBUG
	void Mesh::DebugRender(const maths::mat4& transform)
	{
		if (!m_DebugDraw)
			return;

		debug::DebugRenderer::DrawAABB(m_AABB, transform);
		// debug::DebugRenderer::DrawMesh(this, debug::DebugRenderMeshFlags::NONE, transform);
	}
#endif

} }
