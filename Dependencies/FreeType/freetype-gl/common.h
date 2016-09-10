#pragma once

#ifdef SP_PLATFORM_WINDOWS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif