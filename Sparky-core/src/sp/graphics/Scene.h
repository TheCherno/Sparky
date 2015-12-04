#pragma once

#include <vector>

#include <sp/Types.h>
#include "Mesh.h"

namespace sp { namespace graphics {

	class Renderer3D;

	// TODO: This probably shouldn't be in graphics.
	class Scene
	{
	private:
		std::vector<Mesh*> m_Meshes; // TODO: Replace with component-based Entities!
	public:
		Scene();
		~Scene();
		void Add(Mesh* mesh);
		void Render(Renderer3D& renderer);

		const std::vector<Mesh*>& GetMeshes() const { return m_Meshes; }
	};

} }
