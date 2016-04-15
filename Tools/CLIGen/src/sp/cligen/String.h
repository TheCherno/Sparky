#pragma once

#include <vector>
#include <string>
#include "../SPTypes.h"

typedef std::string String;

namespace sp { namespace cligen {

	std::vector<String> SplitString(const String& string, const String& delimiters);
	std::vector<String> SplitString(const String& string, const char delimiter);
	std::vector<String> Tokenize(const String& string);
	std::vector<String> GetLines(const String& string);

	const char* FindToken(const char* str, const String& token);
	const char* FindToken(const String& string, const String& token);
	int FindStringPosition(const String& string, const String& search, uint offset = 0);
	String RemoveStringRange(const String& string, uint start, uint length);

	String GetBlock(const char* str, const char** outPosition = nullptr);
	String GetBlock(const String& string, uint offset = 0);

	String GetStatement(const char* str, const char** outPosition = nullptr);

	bool StartsWith(const String& string, const String& start);
	bool EndsWith(const String& string, const String& end);
	int NextInt(const String& string);

} }