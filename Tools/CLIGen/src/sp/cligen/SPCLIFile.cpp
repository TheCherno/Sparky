#include "sp.h"

#include "SPCLIFile.h"

namespace sp { namespace cligen {

	SPCLIFile::SPCLIFile(const Class& cppClass, const String& headerFile, const String& implementationFile)
		: m_Header(cppClass), m_Implementation(cppClass), m_HeaderFile(headerFile), m_ImplementationFile(implementationFile)
	{
	}

	void SPCLIFile::Generate()
	{
		m_Header.Generate();
		m_Implementation.Generate();
	}

	void SPCLIFile::Flush()
	{
		m_Header.Flush(m_HeaderFile);
		m_Implementation.Flush(m_ImplementationFile);
	}

} }