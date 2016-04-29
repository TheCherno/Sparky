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

#ifdef SP_DEBUG
	#define SP_DEBUG_METHOD_V(x) x;
#else
	#define SP_DEBUG_METHOD_V(x) x {}
#endif

#if defined(SP_DEBUG) || defined(SP_RELEASE)
	#define SP_DEBUG_VAR(x) x
#else
	#define SP_DEBUG_VAR(x)
#endif

#if defined(SP_DEBUG) || defined(SP_RELEASE)
	#define SP_DEBUG_BLOCK(x) x
#else
	#define SP_DEBUG_BLOCK(x) x
#endif

#define SP_TIMER_BLOCK(x, var) \
	{ \
		Timer timer; \
		x; \
		var = timer.ElapsedMillis(); \
	}

#define SP_TIMER_START() { \
		Timer timer

#define SP_TIMER_STOP(x) \
		x = timer.ElapsedMillis(); \
	}