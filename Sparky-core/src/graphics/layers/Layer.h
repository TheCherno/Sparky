#pragma once

#include <sparky_types.h>

#include "../Renderer2D.h"
#include "../Renderable2D.h"
#include "../Mask.h"

namespace sparky { namespace graphics {

	class Layer
	{
	public:
		Renderer2D* renderer;
	protected:
		std::vector<Renderable2D*> m_Renderables;
		Shader* m_Shader;
		maths::mat4 m_ProjectionMatrix;
	public:
		Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix);
		virtual ~Layer();
		virtual Renderable2D* Add(Renderable2D* renderable);

		inline void SetMask(const Mask* mask) const { renderer->SetMask(mask); }

		virtual void Render();

		inline const std::vector<Renderable2D*>& GetRenderables() const { return m_Renderables; }
	};

} }