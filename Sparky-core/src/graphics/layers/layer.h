#pragma once

#include <sparky_types.h>

#include "../renderer2d.h"
#include "../renderable2d.h"

namespace sparky { namespace graphics {

	class Layer
	{
	protected:
		Renderer2D* m_Renderer;
		std::vector<Renderable2D*> m_Renderables;
		Shader* m_Shader;
		maths::mat4 m_ProjectionMatrix;
	public:
		Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix);
		virtual ~Layer();
		virtual Renderable2D* add(Renderable2D* renderable);

		inline void setMask(const Texture* mask) const { m_Renderer->setMask(mask); }

		virtual void render();

		inline const std::vector<Renderable2D*>& getRenderables() const { return m_Renderables; }
	};

} }