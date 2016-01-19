#pragma once

#include "Light.h"

namespace sp { namespace graphics {

	class SP_API LightSetup
	{
	private:
		std::vector<Light*> m_Lights;
	public:
		LightSetup();
		~LightSetup();

		Light* Add(Light* light);
		void Remove(Light* light);

		inline const std::vector<Light*>& GetLights() const { return m_Lights; }
	};

} }