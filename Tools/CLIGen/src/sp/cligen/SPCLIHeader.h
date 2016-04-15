#pragma once

#include <vector>
#include <string>

#include "SPCLICommon.h"

namespace sp { namespace cligen {

	class SPCLIHeader
	{
	private:
		String m_Buffer;
		Class m_Class;
	public:
		SPCLIHeader(const Class& cppClass);
		void Generate();
		void Flush(const String& file);
	private:
		String Indent(int tabs);
	};

} }