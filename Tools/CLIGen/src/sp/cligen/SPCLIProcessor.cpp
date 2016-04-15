#include "sp.h"
#include "SPCLIProcessor.h"

namespace sp { namespace cligen {

	String ProcessNamespace(const String& ns)
	{
		if (StartsWith(ns, "sp::"))
			return String("sp::cli::") + ns.substr(4, ns.size() - 4);
		return ns;
	}

	String ProcessType(const String& type)
	{
		String result;
		std::vector<String> tokens = Tokenize(type);
		for (int i = 0; i < tokens.size(); i++)
		{
			const String& token = tokens[i];
			if (token == "const")
				continue;

			if (token == "*" || token == "&")
			{
				result = result.substr(0, result.size() - 1);
				result += '^';
			}
			else
			{
				result += ProcessNamespace(token);
			}

			if (i < tokens.size() - 1)
				result += ' ';
		}
		return result;
	}

} }