#include "sp.h"
#include "SPCLIHeader.h"
#include "SPCLIProcessor.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>

namespace sp {
	namespace cligen {

		extern std::map<String, String> g_TypeMappings;

		SPCLIHeader::SPCLIHeader(const Class& cppClass)
			: m_Class(cppClass)
		{
		}

		void SPCLIHeader::Generate()
		{
			String cppFullClassname = "";
			for (String& ns : m_Class.namespaceStack)
				cppFullClassname += ns + "::";
			cppFullClassname += m_Class.name;
			int namespaceCount = 2;
			std::stringstream stream;
			stream << "#pragma once" << std::endl;
			stream << std::endl;
			stream << "namespace sp { namespace cli { ";
			for (String& ns : m_Class.namespaceStack)
			{
				// Ignore standard namespaces
				if (ns == "sparky" || ns == "sp")
					continue;
				stream << "namespace " << ns << " { ";
				namespaceCount++;
			}
			stream << std::endl << std::endl;
			stream << Indent(1) << "public ref class " << m_Class.name << " : public ";
			if (!m_Class.baseClass.empty())
				stream << ProcessNamespace(m_Class.baseClass);
			else
				stream << "ManagedClass<" << cppFullClassname << ">";
			stream << std::endl;
			stream << Indent(1) << "{" << std::endl;
			stream << Indent(1) << "private:" << std::endl;
			stream << Indent(2) << m_Class.name << "(" << cppFullClassname << "* instance);" << std::endl;
			stream << Indent(1) << "public:" << std::endl;

			for (int i = 0; i < m_Class.methods.size(); i++)
			{
				const Method& method = m_Class.methods[i];

				String methodType = method.type;
				if (g_TypeMappings.find(methodType) != g_TypeMappings.end())
					methodType = g_TypeMappings[methodType];
				else
					methodType = ProcessType(methodType);

				stream << Indent(2);
				if (method.methodType == MethodType::CONSTRUCTOR)
					stream << m_Class.name;
				else
					stream << (method.isStatic ? "static " : "") << methodType << " " << method.name.c_str();
				stream << "(";
				for (int i = 0; i < method.parameters.size(); i++)
				{
					String paramType = method.parameters[i].type;
					if (g_TypeMappings.find(paramType) != g_TypeMappings.end())
						paramType = g_TypeMappings[paramType];
					else
						paramType = ProcessType(paramType);
					stream << paramType << " " << method.parameters[i].name;
					if (i < method.parameters.size() - 1)
						stream << ", ";
				}
				stream << ");" << std::endl;
			}
			stream << Indent(1) << "};" << std::endl << std::endl;

			for (int i = 0; i < namespaceCount; i++)
				stream << "} ";

			stream << std::endl;
			m_Buffer = stream.str();
		}

		void SPCLIHeader::Flush(const String& filename)
		{
			FILE* file = fopen(filename.c_str(), "w");
			SP_ASSERT(file);
			int result = fputs(&m_Buffer[0], file);
			fclose(file);
		}

		String SPCLIHeader::Indent(int tabs)
		{
			return String(tabs, '\t');
		}

	}
}