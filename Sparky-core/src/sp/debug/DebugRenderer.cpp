#include "sp/sp.h"
#include "DebugRenderer.h"

#include "sp/graphics/shaders/ShaderFactory.h"
#include "sp/graphics/BatchRenderer2D.h" // TEMP

#include "sp/graphics/Renderer.h"
#include "sp/platform/directx/DXContext.h"

namespace sp { namespace debug {

#define RENDERER_VERTEX_SIZE	sizeof(LineVertex)
#define RENDERER_MAX_LINES		65536
#define RENDERER_BUFFER_SIZE	RENDERER_VERTEX_SIZE * RENDERER_MAX_LINES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_LINES * 2

	using namespace graphics;
	using namespace maths;

	struct LineVertex
	{
		vec3 position;
		uint color;
	};

	static API::Shader* s_Shader;
	static Camera* s_Camera;
	static API::VertexArray* s_VertexArray;
	static API::IndexBuffer* s_IndexBuffer;
	static LineVertex* s_LineVertexPointer;
	static uint s_IndexCount;

	static const uint s_RequiredSystemUniformsCount = 2;
	static const String s_RequiredSystemUniforms[s_RequiredSystemUniformsCount] =
	{
		"sys_ProjectionMatrix",
		"sys_ViewMatrix"
	};

	static const uint sys_ProjectionMatrixIndex = 0;
	static const uint sys_ViewMatrixIndex = 1;

	static std::vector<BR2DSystemUniform> s_SystemUniforms;
	static std::vector<UniformBuffer> s_SystemUniformBuffers;

	void DebugRenderer::Init()
	{
		s_Camera = nullptr;

		s_Shader = ShaderFactory::DebugShader();
		s_Shader->Bind();

		s_SystemUniforms.resize(s_RequiredSystemUniformsCount);
		const API::ShaderUniformBufferList& vssu = s_Shader->GetVSSystemUniforms();
		SP_ASSERT(vssu.size());
		for (uint i = 0; i < vssu.size(); i++)
		{
			API::ShaderUniformBufferDeclaration* ub = vssu[i];
			UniformBuffer buffer(spnew byte[ub->GetSize()], ub->GetSize());
			s_SystemUniformBuffers.push_back(buffer);
			for (API::ShaderUniformDeclaration* decl : ub->GetUniformDeclarations())
			{
				for (uint j = 0; j < s_RequiredSystemUniformsCount; j++)
				{
					if (decl->GetName() == s_RequiredSystemUniforms[j])
						s_SystemUniforms[j] = BR2DSystemUniform(buffer, decl->GetOffset());
				}
			}
		}

		API::VertexBuffer* buffer = API::VertexBuffer::Create(API::BufferUsage::DYNAMIC);
		buffer->Bind();
		buffer->Resize(RENDERER_BUFFER_SIZE);

		API::BufferLayout layout;
		layout.Push<vec3>("POSITION");
		layout.Push<byte>("COLOR", 4, true);
		buffer->SetLayout(layout);

		s_VertexArray = API::VertexArray::Create();
		s_VertexArray->PushBuffer(buffer);

		uint* indices = spnew uint[RENDERER_INDICES_SIZE];
		for (int32 i = 0; i < RENDERER_INDICES_SIZE; i++)
			indices[i] = i;

		s_IndexBuffer = API::IndexBuffer::Create(indices, RENDERER_INDICES_SIZE);
		buffer->Unbind();
		s_VertexArray->Unbind();
		s_IndexCount = 0;
	}

	void DebugRenderer::Shutdown()
	{
		spdel s_Shader;
		spdel s_VertexArray;
		spdel s_IndexBuffer;
	}

	static void Begin()
	{
		s_VertexArray->GetBuffer()->Bind();
		s_LineVertexPointer = s_VertexArray->GetBuffer()->GetPointer<LineVertex>() + s_IndexCount;
	}

	static void End()
	{
		s_VertexArray->GetBuffer()->ReleasePointer();
		s_VertexArray->GetBuffer()->Unbind();
	}

	void DebugRenderer::DrawLineInternal(const maths::vec3& start, const maths::vec3& end, uint color)
	{
		s_LineVertexPointer->position = start;
		s_LineVertexPointer->color = color;
		s_LineVertexPointer++;

		s_LineVertexPointer->position = end;
		s_LineVertexPointer->color = color;
		s_LineVertexPointer++;

		s_IndexCount += 2;

		if (s_IndexCount >= RENDERER_INDICES_SIZE)
		{
			End();
			Present();
			Begin();
		}
	}

	void DebugRenderer::DrawLine(const maths::vec3& start, const maths::vec3& end, uint color)
	{
		Begin();
		DrawLineInternal(start, end, color);
		End();
	}

	void DebugRenderer::DrawAABB(const maths::AABB& aabb, const maths::mat4& transform)
	{
		Begin();
		AABB transformed = aabb.GetTransformed(transform);

		DrawLineInternal(transformed.min, vec3(transformed.max.x, transformed.min.y, transformed.min.z));
		DrawLineInternal(transformed.min, vec3(transformed.min.x, transformed.max.y, transformed.min.z));
		DrawLineInternal(vec3(transformed.min.x, transformed.max.y, transformed.min.z), vec3(transformed.max.x, transformed.max.y, transformed.min.z));
		DrawLineInternal(vec3(transformed.max.x, transformed.min.y, transformed.min.z), vec3(transformed.max.x, transformed.max.y, transformed.min.z));

		DrawLineInternal(vec3(transformed.min.x, transformed.min.y, transformed.max.z), vec3(transformed.max.x, transformed.min.y, transformed.max.z));
		DrawLineInternal(vec3(transformed.min.x, transformed.min.y, transformed.max.z), vec3(transformed.min.x, transformed.max.y, transformed.max.z));
		DrawLineInternal(vec3(transformed.min.x, transformed.max.y, transformed.max.z), vec3(transformed.max.x, transformed.max.y, transformed.max.z));
		DrawLineInternal(vec3(transformed.max.x, transformed.min.y, transformed.max.z), vec3(transformed.max.x, transformed.max.y, transformed.max.z));

		DrawLineInternal(transformed.min, vec3(transformed.min.x, transformed.min.y, transformed.max.z));
		DrawLineInternal(vec3(transformed.max.x, transformed.min.y, transformed.min.z), vec3(transformed.max.x, transformed.min.y, transformed.max.z));
		DrawLineInternal(vec3(transformed.min.x, transformed.max.y, transformed.min.z), vec3(transformed.min.x, transformed.max.y, transformed.max.z));
		DrawLineInternal(vec3(transformed.max.x, transformed.max.y, transformed.min.z), vec3(transformed.max.x, transformed.max.y, transformed.max.z));
		End();
	}

	void DebugRenderer::DrawMesh(const Mesh* mesh, DebugRenderMeshFlags flags, const maths::mat4& transform)
	{
#if 0
		Vertex* vertices = nullptr;
		uint count = mesh->GetDebugData(vertices);
		float scalar = 2.0f;
		Begin();
		for (uint i = 0; i < count; i++)
		{
			Vertex& v = vertices[i];
			vec3 position = transform * v.position;
			DrawLineInternal(vec3(position.x, position.y, position.z), vec3(position.x + v.normal.x * scalar, position.y + v.normal.y * scalar, position.z + v.normal.z * scalar), 0xff0000ff);
			DrawLineInternal(vec3(position.x, position.y, position.z), vec3(position.x + v.binormal.x * scalar, position.y + v.binormal.y * scalar, position.z + v.binormal.z * scalar), 0xff00ff00);
			DrawLineInternal(vec3(position.x, position.y, position.z), vec3(position.x + v.tangent.x * scalar, position.y + v.tangent.y * scalar, position.z + v.tangent.z * scalar), 0xffff0000);
		}
		End();
#endif
	}

	void DebugRenderer::SetCamera(Camera* camera)
	{
		Present();
		s_Camera = camera;

		memcpy(s_SystemUniforms[sys_ProjectionMatrixIndex].buffer.buffer + s_SystemUniforms[sys_ProjectionMatrixIndex].offset, &camera->GetProjectionMatrix(), sizeof(mat4));
		memcpy(s_SystemUniforms[sys_ViewMatrixIndex].buffer.buffer + s_SystemUniforms[sys_ViewMatrixIndex].offset, &camera->GetViewMatrix(), sizeof(mat4));
	}

	void DebugRenderer::Present()
	{
		if (!s_Camera)
			return;

		s_Shader->Bind();
		for (uint i = 0; i < s_SystemUniformBuffers.size(); i++)
			s_Shader->SetVSSystemUniformBuffer(s_SystemUniformBuffers[i].buffer, s_SystemUniformBuffers[i].size, i);

		s_VertexArray->Bind();
		s_IndexBuffer->Bind();
		API::D3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		Renderer::SetDepthTesting(false);
		s_VertexArray->Draw(s_IndexCount);
		s_IndexBuffer->Unbind();
		s_VertexArray->Unbind();
		s_IndexCount = 0;
	}

} }
