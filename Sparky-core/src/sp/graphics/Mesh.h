#pragma once

#include "sp/Common.h"
#include "API/VertexArray.h"
#include "API/IndexBuffer.h"
#include "Material.h"

#include "IRenderable.h"

namespace sp { namespace graphics {

	class Renderer3D;

	struct SP_API Vertex
	{
		maths::vec3 position;
		maths::vec3 normal;
		maths::vec2 uv;
		maths::vec3 binormal;
		maths::vec3 tangent;
	};

	class SP_API Mesh : public IRenderable
	{
	private:
		API::VertexArray* m_VertexArray;
		API::IndexBuffer* m_IndexBuffer;
		MaterialInstance* m_MaterialInstance;
		maths::AABB m_AABB;
#ifdef SP_DEBUG
		bool m_DebugDraw;
#endif
	public:
		Mesh(API::VertexArray* vertexArray, API::IndexBuffer* indexBuffer, MaterialInstance* materialInstance);
		Mesh(const Mesh* mesh);
		~Mesh();

		inline void SetMaterial(MaterialInstance* materialInstance) { m_MaterialInstance = materialInstance; }
		inline MaterialInstance* GetMaterialInstance() const { return m_MaterialInstance; }

		inline const maths::AABB& GetAABB() const { return m_AABB; }

		void Render(Renderer3D& renderer) override;

#ifdef SP_DEBUG
		void DebugRender(const maths::mat4& transform);
		inline void SetDebugDraw(bool debug) { m_DebugDraw = debug; }
#endif
	private:
		void ComputeAABB();
	};

} }
