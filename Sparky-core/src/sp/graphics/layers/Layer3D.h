#pragma once

#include "sp/Common.h"

#include "Layer.h"
#include "../Scene.h"

#include "../ForwardRenderer.h"

namespace sp { namespace graphics {

	class SP_API Layer3D : public Layer
	{
	protected:
		Scene* m_Scene;
		Renderer3D* m_Renderer;
	public:
		Layer3D(Scene* scene, Renderer3D* renderer = new ForwardRenderer());
		~Layer3D();

		virtual void Init();
		virtual void OnInit(Renderer3D& renderer, Scene& scene);

		inline Scene* GetScene() const { return m_Scene; }

		void OnUpdateInternal(const Timestep& ts) override;
		void OnRender() override;
		virtual void OnRender(Renderer3D& renderer);
	protected:
		virtual bool OnResize(uint width, uint height) override;
	};

} }
