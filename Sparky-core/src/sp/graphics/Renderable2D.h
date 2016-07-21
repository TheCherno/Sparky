#pragma once

#include "sp/Common.h"
#include "sp/Types.h"

#include "Renderer2D.h"
#include "API/Texture.h"

#include "API/VertexBuffer.h"
#include "API/IndexBuffer.h"
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
		uint color;
	};

#define RENDERER_VERTEX_SIZE	sizeof(VertexData)

	///
	/// Renderable2D
	///
	/// Base class for 2D renderables, such as Sprites and Labels.
	/// Note that positions are stored as center points with size
	/// being half-width and half-height extents. AABBs in Sparky
	/// on the other hand are min and max coordinates.
	class SP_API Renderable2D
	{
	protected:
		maths::Rectangle m_Bounds;
		uint m_Color;
		std::vector<maths::vec2> m_UVs;
		API::Texture* m_Texture;
		bool m_Visible;
	protected:
		Renderable2D();
	public:
		Renderable2D(const maths::vec2& position, const maths::vec2& size, uint color);
		virtual ~Renderable2D();

		virtual void Submit(Renderer2D* renderer) const;

		inline const maths::vec2& GetPosition() const { return m_Bounds.position; }
		inline const maths::vec2& GetSize() const { return m_Bounds.size; }

		inline const maths::Rectangle& GetBounds() const { return m_Bounds; }
		inline maths::Rectangle& GetBounds() { return m_Bounds; }

		inline maths::AABB GetBoundingBox() const { return m_Bounds; }
		inline const uint GetColor() const { return m_Color; }

		inline API::Texture* GetTexture() const { return m_Texture; }
		inline const std::vector<maths::vec2>& GetUVs() const { return m_UVs; }

		inline bool IsVisible() const { return m_Visible; }

		inline void SetPosition(const maths::vec2& position) { m_Bounds.position = position; }
		inline void SetSize(const maths::vec2& size) { m_Bounds.size = size; }
		inline void SetColor(uint color) { m_Color = color; }
		void SetColor(const maths::vec4& color);

		inline void SetVisible(bool visible) { m_Visible = visible; }
	public:
		static const std::vector<maths::vec2>& GetDefaultUVs();
	};

} }