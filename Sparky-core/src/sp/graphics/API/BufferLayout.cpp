#include "sp/sp.h"
#include "BufferLayout.h"

namespace sp { namespace graphics { namespace API {

	BufferLayout::BufferLayout()
		: m_Size(0)
	{
	}

	void BufferLayout::Push(const String& name, uint type, uint size, uint count, bool normalized)
	{
		m_Layout.push_back({ name, type, size, count, m_Size, normalized });
		m_Size += size * count;
	}

	const BufferElement* BufferLayout::GetElement(const String& name) const
	{
		for (const BufferElement& element : m_Layout)
		{
			if (name == element.name)
				return &element;
		}
		return nullptr;
	}

} } }