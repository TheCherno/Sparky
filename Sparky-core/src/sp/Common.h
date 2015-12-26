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

// Common defines
#define BIT(x) (1 << x)

#define METHOD_1(x) std::bind(x, this, std::placeholders::_1)
#define METHOD(x) METHOD_1(x)