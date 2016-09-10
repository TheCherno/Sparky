#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/utils/Log.h"
#include "sp/Types.h"
#include "sp/maths/maths.h"

#include "Context.h"

#include "sp/platform/opengl/GLTypes.h"
#include "sp/platform/directx/DXTypes.h"

namespace sp { namespace graphics { namespace API {

	struct SP_API BufferElement
	{
		String name;
		uint type;
		uint size;
		uint count;
		uint offset;
		bool normalized;
	};

	class SP_API BufferLayout
	{
	private:
		uint m_Size;
		std::vector<BufferElement> m_Layout;
	public:
		BufferLayout();

		template<typename T>
		void Push(const String& name, uint count = 1, bool normalized = false)
		{
			SP_ASSERT(false, "Unkown type!");
		}


		inline const std::vector<BufferElement>& GetLayout() const { return m_Layout; }
		inline uint GetStride() const { return m_Size; }
	private:
		void Push(const String& name, uint type, uint size, uint count, bool normalized);
	};

	template<> void BufferLayout::Push<float>(const String& name, uint count, bool normalized);
	template<> void BufferLayout::Push<uint>(const String& name, uint count, bool normalized);
	template<> void BufferLayout::Push<byte>(const String& name, uint count, bool normalized);
	template<> void BufferLayout::Push<maths::vec2>(const String& name, uint count, bool normalized);
	template<> void BufferLayout::Push<maths::vec3>(const String& name, uint count, bool normalized);
	template<> void BufferLayout::Push<maths::vec4>(const String& name, uint count, bool normalized);

} } }