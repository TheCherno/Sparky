#pragma once

#include <sp/Types.h>

#include "Renderer2D.h"
#include "Texture.h"

#include "buffers/Buffer.h"
#include "buffers/IndexBuffer.h"
#include "shaders/Shader.h"

#include "../maths/maths.h"

namespace sp { namespace graphics {

	struct VertexData
	{
		maths::vec3 vertex;
		maths::vec2 uv;
		maths::vec2 mask_uv;
		float tid;
		float mid;
		unsigned int color;
	};

#define RENDERER_VERTEX_SIZE	sizeof(VertexData)

	class Renderable2D
	{
	protected:
		maths::vec3 m_Position;
		maths::vec2 m_Size;
		unsigned int m_Color;
		std::vector<maths::vec2> m_UV;
		Texture* m_Texture;
	protected:
		Renderable2D() : m_Texture(nullptr) { SetUVDefaults(); }
	public:
		Renderable2D(maths::vec3 position, maths::vec2 size, unsigned int color)
			: m_Position(position), m_Size(size), m_Color(color), m_Texture(nullptr)
		{ SetUVDefaults(); }

		virtual ~Renderable2D() { }

		virtual void Submit(Renderer2D* renderer) const
		{
			renderer->Submit(this);
		}

		void SetColor(unsigned int color) { m_Color = color; }
		void SetColor(const maths::vec4& color)
		{ 
			uint r = (uint)(color.x * 255.0f);
			uint g = (uint)(color.y * 255.0f);
			uint b = (uint)(color.z * 255.0f);
			uint a = (uint)(color.w * 255.0f);

			m_Color = a << 24 | b << 16 | g << 8 | r;
		}

		inline const maths::vec3& GetPosition() const { return m_Position; }
		inline const maths::vec2& GetSize() const { return m_Size; }
		inline const unsigned int GetColor() const { return m_Color; }
		inline const std::vector<maths::vec2>& GetUV() const { return m_UV; }

		inline const GLuint GetTID() const { return m_Texture ? m_Texture->GetID() : 0; }
		inline const Texture* GetTexture() const { return m_Texture; }
	private:
		void SetUVDefaults()
		{
			m_UV.push_back(maths::vec2(0, 0));
			m_UV.push_back(maths::vec2(0, 1));
			m_UV.push_back(maths::vec2(1, 1));
			m_UV.push_back(maths::vec2(1, 0));
		}
	};

} }