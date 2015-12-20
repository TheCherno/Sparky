#pragma once

#include "sp/Common.h"
#include "buffers/VertexArray.h"
#include "buffers/IndexBuffer.h"
#include "Material.h"

#include "IRenderable.h"

namespace sp { namespace graphics {

	class Renderer3D;

	struct SP_API Vertex
	{
		maths::vec3 position;
		maths::vec3 normal;
		maths::vec2 uv;
	};

	class SP_API Mesh : public IRenderable
	{
	private:
		VertexArray* m_VertexArray;
		IndexBuffer* m_IndexBuffer;
		MaterialInstance* m_MaterialInstance;
	public:
		Mesh(VertexArray* vertexArray, IndexBuffer* indexBuffer, MaterialInstance* materialInstance);
		~Mesh();

		inline void SetMaterial(MaterialInstance* materialInstance) { m_MaterialInstance = materialInstance; }
		inline MaterialInstance* GetMaterialInstance() const { return m_MaterialInstance; }

		void Render(Renderer3D& renderer) override;
	};

} }
