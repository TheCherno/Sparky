#include "sp/sp.h"

#include <Windows.h>
#include "sp/utils/Log.h"

namespace sp { namespace internal {

	void PlatformLogMessage(uint level, const char* message)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (level)
		{
		case SPARKY_LOG_LEVEL_FATAL:
			SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case SPARKY_LOG_LEVEL_ERROR:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case SPARKY_LOG_LEVEL_WARN:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case SPARKY_LOG_LEVEL_IMPORTANT:
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		default:
			break;
		}
		printf("%s", message);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	}

	SP_API void PlatformLogMessage(uint level, String message)
	{
		return PlatformLogMessage(level, message.c_str());
	}

	SP_API void PlatformLogMessage(const char* message)
	{
		return PlatformLogMessage(3, message);
	}

	SP_API void PlatformLogMessage(String message)
	{
		return PlatformLogMessage(3, message.c_str());
	}

} }