#pragma once

#include "Camera.h"

namespace sp { namespace graphics {

	class SP_API OrthographicCamera : public Camera
	{
	private:

	public:
		OrthographicCamera(const maths::mat4& projectionMatrix);
		~OrthographicCamera();
		void Focus() override;
		void Update() override;

	};

} }
