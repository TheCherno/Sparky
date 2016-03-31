#pragma once

#include "sp/graphics/API/Context.h"
#include "GLCommon.h"

namespace sp { namespace graphics { namespace API {

	class GLContext : public Context
	{
	public:
		GLContext(WindowProperties properties, void* deviceContext);

		void Present();
	public:
		inline static GLContext* Get() { return (GLContext*)s_Context; }
	};

} } }