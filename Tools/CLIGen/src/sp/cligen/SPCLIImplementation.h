#pragma once

#include "SPCLICommon.h"

namespace sp { namespace cligen {

	class SPCLIImplementation
	{
	private:
		String m_Buffer;
		Class m_Class;
		uint m_Indent;
	public:
		SPCLIImplementation(const Class& cppClass);
		void Generate();
		void Flush(const String& file);
	private:
		String Indent();
	};


} }