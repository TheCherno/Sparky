#pragma once

#include "Renderer3D.h"

namespace sp { namespace graphics {

	class ForwardRenderer : public Renderer3D
	{
	private:

	public:
		ForwardRenderer();
		void Init() override;
		void Begin() override;
		void Submit(Mesh* mesh) override;
		void End() override;
		void Present() override;
	};

} }
