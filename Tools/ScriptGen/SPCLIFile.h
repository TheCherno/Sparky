#pragma once

#include <vector>
#include <string>

#include "SPCLICommon.h"
#include "SPCLIHeader.h"
#include "SPCLIImplementation.h"

namespace sp {
	namespace cligen {

		class SPCLIFile
		{
		private:
			SPCLIHeader m_Header;
			SPCLIImplementation m_Implementation;
			String m_HeaderFile, m_ImplementationFile;
		public:
			SPCLIFile(const Class& cppClass, const String& headerFile, const String& implementationFile);
			void Generate();
			void Flush();
		};


	}
}