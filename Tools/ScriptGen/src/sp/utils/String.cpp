#include "sp.h"

#include "String.h"

namespace sp { namespace gen {

	std::vector<String> SplitString(const String& string, const String& delimiters)
	{
		size_t start = 0;
		size_t end = string.find_first_of(delimiters);

		std::vector<String> result;

		while (end <= String::npos)
		{
			String token = string.substr(start, end - start);
			if (!token.empty())
				result.push_back(token);

			if (end == String::npos)
				break;

			start = end + 1;
			end = string.find_first_of(delimiters, start);
		}

		return result;
	}

	std::vector<String> SplitString(const String& string, const char delimiter)
	{
		return SplitString(string, String(1, delimiter));
	}

	std::vector<String> Tokenize(const String& string)
	{
		return SplitString(string, " \t\n");
	}

	std::vector<String> GetLines(const String& string)
	{
		return SplitString(string, "\n");
	}

	const char* FindToken(const char* str, const String& token)
	{
		const char* t = str;
		while (t = strstr(t, token.c_str()))
		{
			bool left = str == t || isspace(t[-1]);
			bool right = !t[token.size()] || isspace(t[token.size()]);
			if (left && right)
				return t;

			t += token.size();
		}
		return nullptr;
	}

	const char* FindToken(const String& string, const String& token)
	{
		return FindToken(string.c_str(), token);
	}

	int FindStringPosition(const String& string, const String& search, uint offset)
	{
		const char* str = string.c_str() + offset;
		const char* found = strstr(str, search.c_str());
		if (found == nullptr)
			return -1;
		return (int)(found - str) + offset;
	}

	String RemoveStringRange(const String& string, uint start, uint length)
	{
		String result = string;
		return result.erase(start, length);
	}

	String GetBlock(const char* str, const char** outPosition)
	{
		const char* end = strstr(str, "}");
		if (!end)
			return String(str);

		if (outPosition)
			*outPosition = end;
		uint length = end - str + 1;
		return String(str, length);
	}

	String GetBlock(const String& string, uint offset)
	{
		const char* str = string.c_str() + offset;
		return GetBlock(str);
	}

	String GetStatement(const char* str, const char** outPosition)
	{
		const char* end = strstr(str, ";");
		if (!end)
			return String(str);

		if (outPosition)
			*outPosition = end;
		uint length = end - str + 1;
		return String(str, length);
	}

	bool StartsWith(const String& string, const String& start)
	{
		return string.find(start) == 0;
	}

	bool EndsWith(const String& string, const String& end)
	{
		if (end.size() > string.size())
			return false;
		return std::equal(string.begin() + string.size() - end.size(), string.end(), end.begin());
	}

	int NextInt(const String& string)
	{
		const char* str = string.c_str();
		for (uint i = 0; i < string.size(); i++)
		{
			if (isdigit(string[i]))
				return atoi(&string[i]);
		}
		return -1;
	}

	bool IgnoreCaseEquals(const String& a, const String& b)
	{
		unsigned int sz = a.size();
		if (b.size() != sz)
			return false;
		for (unsigned int i = 0; i < sz; ++i)
			if (tolower(a[i]) != tolower(b[i]))
				return false;
		return true;
	}

	bool Equals(const String& a, const String& b)
	{
		unsigned int sz = a.size();
		if (b.size() != sz)
			return false;
		for (unsigned int i = 0; i < sz; ++i)
			if (a[i] != b[i])
				return false;
		return true;
	}

} }