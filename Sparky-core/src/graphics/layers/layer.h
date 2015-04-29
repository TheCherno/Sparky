#pragma once

#include "../renderer2d.h"
#include "../renderable2d.h"

namespace sparky { namespace graphics {

	struct Line
	{
		maths::vec3 start;
		maths::vec3 end;
		unsigned int color;
	};

	class Layer
	{
	protected:
		Renderer2D* m_Renderer;
		std::vector<Renderable2D*> m_Renderables, m_TemporaryRenderables;
		std::vector<Line> m_Lines;
		Shader* m_Shader;
		maths::mat4 m_ProjectionMatrix;
	public:
		Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix);
		virtual ~Layer();
		virtual void add(Renderable2D* renderable);
		virtual void setLineThickness(float thickness);
		virtual void drawLine(const maths::vec3& start, const maths::vec3& end, unsigned int color);
		virtual void render();

		inline const std::vector<Renderable2D*>& getRenderables() const { return m_Renderables; }
	};

} }