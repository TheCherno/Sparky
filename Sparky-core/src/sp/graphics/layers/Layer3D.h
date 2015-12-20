#pragma once

#include "sp/Common.h"

#include "Layer.h"
#include "../Scene.h"

namespace sp { namespace graphics {

	class SP_API Layer3D : public Layer
	{
	protected:
		Scene* m_Scene;
		Renderer3D* m_Renderer;
	public:
		Layer3D(Scene* scene);
		~Layer3D();

		virtual void Init();
		virtual void OnInit(Renderer3D& renderer, Scene& scene);

		inline Scene* GetScene() const { return m_Scene; }

		void OnRender() override;
		virtual void OnRender(Renderer3D& renderer);
	};

} }
