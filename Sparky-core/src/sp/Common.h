#pragma once
#pragma warning (disable:4251)

#ifdef SP_PLATFORM_WINDOWS
	#ifdef SP_CORE_DLL
		#define SP_API __declspec(dllexport)
	#else
		#define SP_API __declspec(dllimport)
	#endif
#else
	#define SP_API
#endif