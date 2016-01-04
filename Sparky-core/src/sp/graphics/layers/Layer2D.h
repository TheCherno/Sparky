#pragma once

#include "Layer.h"

#include "sp/Common.h"
#include "../../events/Event.h"
#include "../Renderable2D.h"
#include "../Mask.h"

namespace sp { namespace graphics {

	class SP_API Layer2D : public Layer
	{
	private:
		Renderer2D* m_Renderer;
	protected:
		std::vector<Renderable2D*> m_Renderables;
		std::vector<Renderable2D*> m_SubmittedRenderables;
		Shader* m_Shader;
		maths::mat4 m_ProjectionMatrix;
	public:
		// TODO: Replace Shader with Material
		Layer2D(Shader* shader, const maths::mat4& projectionMatrix);
		virtual ~Layer2D();

		virtual void Init();
		virtual void OnInit(Renderer2D& renderer, Shader& shader);

		inline void SetMask(const Mask* mask) const { m_Renderer->SetMask(mask); }
		virtual Renderable2D* Add(Renderable2D* renderable);
		inline const std::vector<Renderable2D*>& GetRenderables() const { return m_Renderables; }

		virtual Renderable2D* Submit(Renderable2D* renderable);

		virtual void OnRender(Renderer2D& renderer);
		void OnRender() override;
	};

} }

