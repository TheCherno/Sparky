#pragma once

#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/String.h"

#include "RenderBuffer.h"

namespace sp { namespace graphics {

	class Framebuffer
	{
	public:
		enum class Type
		 {
			NONE = 0, SCREEN = 1, RGBA = 2, DEPTH = 3
		};
	private:
		String m_DebugName;
		uint m_Width, m_Height;
		Type m_Type;
	public:
		Framebuffer(uint width, uint type, const String& debugName = "");

		void Attach(RenderBuffer* buffer);
	};

} }
