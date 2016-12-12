#include "sp/sp.h"

#include <Windows.h>
#include <fstream>
#include <cstdlib>
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
		//These Functions Just Call The Normal Method
		//This One Has String Support
		return PlatformLogMessage(level, message.c_str());
	}

	SP_API void PlatformLogMessage(const char* message)
	{
		//This One Calls The Normal Info Level By Default
		return PlatformLogMessage(3, message);
	}

	SP_API void PlatformLogMessage(String message)
	{
		//Same As Above With String Support
		return PlatformLogMessage(3, message.c_str());
	}

	SP_API void PlatformFileMessage(String filename, String message)
	{
		//For People Who Like Strings
		PlatformFileMessage(filename, message.c_str());
	}

	SP_API void PlatformFileMessage(String filename, const char * message)
	{
		std::ofstream outputFile;
		outputFile.open(filename);
		if (outputFile.fail())
		{
			PlatformLogMessage(SPARKY_LOG_LEVEL_FATAL, "Error In Working With The File\n");
			perror((filename.c_str()));
			system("PAUSE");
			//If There Is A "Proper" Exit Code To Be Put Here, Put It In.
			exit(67);
		}
		outputFile << message;
		outputFile.close();
	}

	SP_API String ReadFile(String filename)
	{
		return ReadFile(filename.c_str());
	}

	SP_API String ReadFile(const char * filename)
	{
		//Opens And Returns The File's Content
		std::ifstream opener;
		opener.open(filename);
		if (opener.fail())
		{
			PlatformLogMessage(SPARKY_LOG_LEVEL_FATAL, "Error In Opening The File\n");
			perror(filename);
			system("PAUSE");
			//If There Is A "Proper" Exit Code To Be Put Here, Put It In.
			exit(76);
		}
		String buffer;
		String returner;
		while (getline(opener, buffer))
		{
			returner.append(buffer + "\n");
		}
		opener.close();
		return returner;
	}

} }
