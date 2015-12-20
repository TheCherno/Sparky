#include "sp/sp.h"
#include "BufferLayout.h"

namespace sp { namespace graphics {

	BufferLayout::BufferLayout()
	{
	}

	void BufferLayout::Push(const String& name, uint type, uint size, uint count, bool normalized)
	{
		m_Layout.push_back({ name, type, size, count, 0, normalized });
		BufferLayoutType& layoutType = m_Layout.back();
		if (m_Layout.size() > 1)
			layoutType.offset = m_Layout[m_Layout.size() - 2].offset + m_Layout[m_Layout.size() - 2].size;

		m_Size = layoutType.offset + layoutType.size;
	}

} }