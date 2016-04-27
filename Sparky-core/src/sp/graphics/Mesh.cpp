#include "sp/sp.h"
#include "Mesh.h"

#include "Renderer3D.h"

#include <GL/glew.h>

#include "sp/debug/DebugRenderer.h"

namespace sp { namespace graphics {

	Mesh::Mesh(API::VertexArray* vertexArray, API::IndexBuffer* indexBuffer, MaterialInstance* materialInstance)
		: m_VertexArray(vertexArray), m_IndexBuffer(indexBuffer), m_MaterialInstance(materialInstance)
	{
#ifdef SP_DEBUG
		m_DebugVertexData = nullptr;
		m_DebugVertexDataCount = 0;
		m_DebugDraw = false;
#endif
	}

	Mesh::Mesh(const Mesh* mesh)
		: m_VertexArray(mesh->m_VertexArray), m_IndexBuffer(mesh->m_IndexBuffer), m_MaterialInstance(mesh->m_MaterialInstance)
	{
#ifdef SP_DEBUG
		m_DebugVertexData = mesh->m_DebugVertexData;
		m_DebugVertexDataCount = mesh->m_DebugVertexDataCount;
		m_DebugDraw = mesh->m_DebugDraw;
#endif
	}

	Mesh::~Mesh()
	{
		delete m_VertexArray;
		delete m_IndexBuffer;
		delete m_MaterialInstance;

#ifdef SP_DEBUG
		delete[] m_DebugVertexData;
#endif
	}

	void Mesh::Render(Renderer3D& renderer)
	{
		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
		m_VertexArray->Draw(m_IndexBuffer->GetCount());
		m_IndexBuffer->Unbind();
		m_VertexArray->Unbind();
	}

#ifdef SP_DEBUG
	void Mesh::DebugRender(const maths::mat4& transform)
	{
		if (!m_DebugDraw)
			return;

		debug::DebugRenderer::DrawMesh(this, debug::DebugRenderMeshFlags::NONE, transform);
	}
#endif

} }
