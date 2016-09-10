#include "sp/sp.h"

#include <android/log.h>
#include "sp/utils/Log.h"

namespace sp { namespace internal {

	static int SPLogLevelToAndroid(uint level)
	{
		switch (level)
		{
			case SPARKY_LOG_LEVEL_FATAL:	return ANDROID_LOG_FATAL;
			case SPARKY_LOG_LEVEL_ERROR:	return ANDROID_LOG_ERROR;
			case SPARKY_LOG_LEVEL_WARN:		return ANDROID_LOG_WARN;
			case SPARKY_LOG_LEVEL_INFO:		return ANDROID_LOG_INFO;
		}
		SP_ASSERT(false, "");
		return -1;
	}

	void PlatformLogMessage(uint level, const char* message)
	{
		__android_log_print(SPLogLevelToAndroid(level), "Sparky", "%s", message);
	}

} }