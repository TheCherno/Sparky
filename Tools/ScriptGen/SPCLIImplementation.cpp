#include "sp.h"

#include "SPCLIImplementation.h"
#include "SPCLIProcessor.h"

namespace sp {
	namespace cligen {

		extern std::map<String, String> g_TypeMappings;

		SPCLIImplementation::SPCLIImplementation(const Class& cppClass)
			: m_Class(cppClass), m_Indent(0)
		{
		}

		void SPCLIImplementation::Generate()
		{
			String cppFullClassname = "";
			for (String& ns : m_Class.namespaceStack)
				cppFullClassname += ns + "::";
			cppFullClassname += m_Class.name;
			int namespaceCount = 2;
			std::stringstream stream;
			stream << "#include \"" << ".h" << "\"" << std::endl;
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

			m_Indent++;

			stream << std::endl << std::endl;

			for (int i = 0; i < m_Class.methods.size(); i++)
			{
				const Method& method = m_Class.methods[i];

				String methodType = method.type;
				if (g_TypeMappings.find(methodType) != g_TypeMappings.end())
					methodType = g_TypeMappings[methodType];
				else
					methodType = ProcessType(methodType);

				stream << Indent();
				if (method.methodType == MethodType::CONSTRUCTOR)
					stream << m_Class.name;
				else
					stream << methodType << " " << m_Class.name << "::" << method.name;
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
				stream << ")" << std::endl;
				stream << Indent() << "{" << std::endl;
				m_Indent++;
				stream << Indent();
				if (method.methodType == MethodType::CONSTRUCTOR)
				{
					stream << "m_Instance = new " << cppFullClassname << "(";
				}
				else
				{
					if (methodType != "void")
						stream << "return ";
					if (method.isStatic)
						stream << cppFullClassname << "::";
					else
						stream << "m_Instance->";
					stream << method.name << "(";
				}
				for (int i = 0; i < method.parameters.size(); i++)
				{
					// TODO: Marshalling, conversion, etc. here
					stream << method.parameters[i].name;
					if (i < method.parameters.size() - 1)
						stream << ", ";
				}
				stream << ");" << std::endl;
				m_Indent--;
				stream << Indent() << "}" << std::endl << std::endl;
			}

			m_Indent--;
			stream << std::endl;

			for (int i = 0; i < namespaceCount; i++)
				stream << "} ";

			stream << std::endl;
			m_Buffer = stream.str();
		}

		void SPCLIImplementation::Flush(const std::string& filename)
		{
			FILE* file = fopen(filename.c_str(), "w");
			int result = fputs(&m_Buffer[0], file);
			fclose(file);
		}

		String SPCLIImplementation::Indent()
		{
			return String(m_Indent, '\t');
		}

	}
}