#pragma once

#include "sp/Common.h"
#include "sp/Types.h"

#include "Renderer2D.h"
#include "Texture.h"

#include "buffers/Buffer.h"
#include "buffers/IndexBuffer.h"
#include "shaders/Shader.h"

#include "../maths/maths.h"

namespace sp { namespace graphics {

	struct SP_API VertexData
	{
		maths::vec3 vertex;
		maths::vec2 uv;
		maths::vec2 mask_uv;
		float tid;
		float mid;
		unsigned int color;
	};

#define RENDERER_VERTEX_SIZE	sizeof(VertexData)

	class SP_API Renderable2D
	{
	protected:
		maths::vec3 m_Position;
		maths::vec2 m_Size;
		uint m_Color;
		std::vector<maths::vec2> m_UVs;
		Texture* m_Texture;
		bool m_Visible;
	protected:
		Renderable2D() : m_Texture(nullptr) { m_UVs = GetDefaultUVs(); }
	public:
		Renderable2D(const maths::vec3& position, const maths::vec2& size, uint color)
			: m_Position(position), m_Size(size), m_Color(color), m_Texture(nullptr), m_Visible(true)
		{ m_UVs = GetDefaultUVs(); }

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
		inline const std::vector<maths::vec2>& GetUV() const { return m_UVs; }

		inline const uint GetTID() const { return m_Texture ? m_Texture->GetID() : 0; }
		inline const Texture* GetTexture() const { return m_Texture; }

		inline bool IsVisible() const { return m_Visible; }
		inline void SetVisible(bool visible) { m_Visible = visible; }
	public:
		static std::vector<maths::vec2> GetDefaultUVs()
		{
			static std::vector<maths::vec2> results;
			if (!results.size())
			{
				results.push_back(maths::vec2(0, 0));
				results.push_back(maths::vec2(0, 1));
				results.push_back(maths::vec2(1, 1));
				results.push_back(maths::vec2(1, 0));
			}
			return results;
		}
	};

} }