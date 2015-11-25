#pragma once

#include "Layer.h"

#include "../../events/Event.h"
#include "../Renderable2D.h"
#include "../Mask.h"

namespace sp { namespace graphics {

	class Layer2D : public Layer
	{
	private:
		Renderer2D* m_Renderer;
	protected:
		std::vector<Renderable2D*> m_Renderables;
		Shader* m_Shader;
		maths::mat4 m_ProjectionMatrix;
	public:
		Layer2D(Shader* shader, const maths::mat4& projectionMatrix);
		virtual ~Layer2D();

		virtual void Init();
		virtual void OnInit(Renderer2D& renderer, Shader& shader);

		inline void SetMask(const Mask* mask) const { m_Renderer->SetMask(mask); }
		virtual Renderable2D* Add(Renderable2D* renderable);
		inline const std::vector<Renderable2D*>& GetRenderables() const { return m_Renderables; }

		virtual void OnRender(Renderer2D& renderer);
		void OnRender() override;
	};

} }

