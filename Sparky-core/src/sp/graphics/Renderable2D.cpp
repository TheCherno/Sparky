#include "sp/sp.h"
#include "Renderable2D.h"

namespace sp { namespace graphics {

	using namespace maths;

	Renderable2D::Renderable2D()
		: m_Texture(nullptr)
	{
		m_UVs = GetDefaultUVs();
	}

	Renderable2D::Renderable2D(const maths::vec2& position, const maths::vec2& size, uint color)
		: m_Bounds(position, size), m_Color(color), m_Texture(nullptr), m_Visible(true)
	{
		m_UVs = GetDefaultUVs();
	}

	Renderable2D::~Renderable2D()
	{
	}

	void Renderable2D::Submit(Renderer2D* renderer) const
	{
		renderer->Submit(this);
	}

	void Renderable2D::SetColor(const vec4& color)
	{
		uint r = (uint)(color.x * 255.0f);
		uint g = (uint)(color.y * 255.0f);
		uint b = (uint)(color.z * 255.0f);
		uint a = (uint)(color.w * 255.0f);

		m_Color = a << 24 | b << 16 | g << 8 | r;
	}

	const std::vector<maths::vec2>& Renderable2D::GetDefaultUVs()
	{
		static std::vector<maths::vec2> results;
		if (!results.size())
		{
			results.push_back(maths::vec2(0, 1));
			results.push_back(maths::vec2(1, 1));
			results.push_back(maths::vec2(1, 0));
			results.push_back(maths::vec2(0, 0));
		}
		return results;
	}

} }