#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/String.h"

namespace sp {

	class SP_API Platform
	{
	private:
		String m_Name;
		String m_Architecture;

	public:
		Platform(const String& name, const String& architecture)
			: m_Name(name), m_Architecture(architecture) {}
		~Platform() {}

		inline const String& GetName() const { return m_Name; }
		inline const String& GetArchitecture() const { return m_Architecture; }

		inline bool IsPlatform(const String& nameOrArchitecture) const
		{
			return (nameOrArchitecture == m_Name || nameOrArchitecture == m_Architecture);
		}

	};

}
