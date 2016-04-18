#pragma once

#include "sp/Common.h"
#include "sp/Types.h"

#include "sp/maths/vec2.h"
#include "sp/maths/Rectangle.h"
#include "sp/events/Events.h"

#define SPARKY_LOG_LEVEL_FATAL 0
#define SPARKY_LOG_LEVEL_ERROR 1
#define SPARKY_LOG_LEVEL_WARN  2
#define SPARKY_LOG_LEVEL_INFO  3

#ifdef MOUSE_MOVED
	#undef MOUSE_MOVED // Defined in wincon.h
#endif

namespace std
{
	template <typename T>
	string to_string(const T& t)
	{
		return string("[Unsupported type (") + typeid(T).name() + string(")!] (to_string)");
	}
}

//
// Work in progress!
//
// -------------------------------
//			TODO: 
// -------------------------------
//	- Better container type logging
//	- Better platform support
//	- Logging to other destinations (eg. files)
//	- Include (almost) ALL Sparky class types
//	- More...
namespace sp { namespace internal {
	
	static char to_string_buffer[1024 * 10];
	static char sprintf_buffer[1024 * 10];

	SP_API void PlatformLogMessage(uint level, const char* message);

	template <class T>
	struct has_iterator
	{
		template<class U> static char(&test(typename U::iterator const*))[1];
		template<class U> static char(&test(...))[2];
		static const bool value = (sizeof(test<T>(0)) == 1);
	};

	template <typename T>
	static const char* to_string(const T& t)
	{
		return to_string_internal<T>(t, std::integral_constant<bool, has_iterator<T>::value>());
	}

	template <>
	static const char* to_string<char>(const char& t)
	{
		return &t;
	}

	template <>
	static const char* to_string<char*>(char* const& t)
	{
		return t;
	}

	template <>
	static const char* to_string<unsigned char const*>(unsigned char const* const& t)
	{
		return (const char*)t;
	}

	template <>
	static const char* to_string<wchar_t*>(wchar_t* const& t)
	{
		wcstombs(sprintf_buffer, t, 1024 * 10);
		return sprintf_buffer;
	}

	template <>
	static const char* to_string<const wchar_t*>(const wchar_t* const& t)
	{
		wcstombs(sprintf_buffer, t, 1024 * 10);
		return sprintf_buffer;
	}

	template <>
	static const char* to_string<const char*>(const char* const& t)
	{
		return t;
	}

	template <>
	static const char* to_string<String>(const String& t)
	{
		return t.c_str();
	}

	template <>
	static const char* to_string<bool>(const bool& t)
	{
		return t ? "true" : "false";
	}

	template <>
	static const char* to_string<maths::vec2>(const maths::vec2& t)
	{
		// TODO: sprintf
		String string = String("vec2: (") + StringFormat::ToString(t.x) + ", " + StringFormat::ToString(t.y) + ")";
		char* result = new char[string.length()];
		strcpy(result, &string[0]);
		return result;
	}

	template <>
	static const char* to_string<maths::vec3>(const maths::vec3& t)
	{
		// TODO: sprintf
		String string = String("vec3: (") + StringFormat::ToString(t.x) + ", " + StringFormat::ToString(t.y) + ", " + StringFormat::ToString(t.z) + ")";
		char* result = new char[string.length()];
		strcpy(result, &string[0]);
		return result;
	}

	template <>
	static const char* to_string<maths::Rectangle>(const maths::Rectangle& r)
	{
		sprintf(sprintf_buffer, "Rectangle: (%f, %f, %f, %f)", r.x, r.y, r.width, r.height);
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	static const char* to_string<events::KeyPressedEvent>(const events::KeyPressedEvent& e)
	{
		sprintf(sprintf_buffer, "KeyPressedEvent: (%d, %d)", e.GetKeyCode(), e.GetRepeat());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	static const char* to_string<events::KeyReleasedEvent>(const events::KeyReleasedEvent& e)
	{
		sprintf(sprintf_buffer, "KeyReleasedEvent: (%d)", e.GetKeyCode());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	static const char* to_string<events::MousePressedEvent>(const events::MousePressedEvent& e)
	{
		sprintf(sprintf_buffer, "MousePressedEvent: (%d, %f, %f)", e.GetButton(), e.GetX(), e.GetY());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	static const char* to_string<events::MouseReleasedEvent>(const events::MouseReleasedEvent& e)
	{
		sprintf(sprintf_buffer, "MouseReleasedEvent: (%d, %f, %f)", e.GetButton(), e.GetX(), e.GetY());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	static const char* to_string<events::MouseMovedEvent>(const events::MouseMovedEvent& e)
	{
		sprintf(sprintf_buffer, "MouseMovedEvent: (%f, %f, %s)", e.GetX(), e.GetY(), e.IsDragged() ? "true" : "false");
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	static const char* to_string<events::Event>(const events::Event& e)
	{
		sprintf(sprintf_buffer, "Event: %s (%d)", events::Event::TypeToString(e.GetType()).c_str(), e.GetType());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	static const char* to_string<events::Event*>(events::Event* const& e)
	{
		using namespace events;

		switch (e->GetType())
		{
		case Event::Type::KEY_PRESSED:
			return to_string(*(KeyPressedEvent*)e);
		case Event::Type::KEY_RELEASED:
			return to_string(*(KeyReleasedEvent*)e);
		case Event::Type::MOUSE_PRESSED:
			return to_string(*(MousePressedEvent*)e);
		case Event::Type::MOUSE_RELEASED:
			return to_string(*(MouseReleasedEvent*)e);
		case Event::Type::MOUSE_MOVED:
			return to_string(*(MouseMovedEvent*)e);
		}
		return "Unkown Event!";
	}

	template <typename T>
	static String format_iterators(T& begin, T& end)
	{
		String result;
		bool first = true;
		while (begin != end)
		{
			if (!first)
				result += ", ";
			result += to_string(*begin);
			first = false;
			begin++;
		}
		return result;
	}

	template <typename T>
	static const char* to_string_internal(const T& v, const std::true_type& ignored)
	{
		sprintf(to_string_buffer, "Container of size: %d, contents: %s", v.size(), format_iterators(v.begin(), v.end()).c_str());
		return to_string_buffer;
	}

	template <typename T>
	static const char* to_string_internal(const T& t, const std::false_type& ignored)
	{
		auto x = StringFormat::ToString(t);
		return strcpy(to_string_buffer, x.c_str());
	}

	template<typename X, typename Y>
	static const char* to_string(const std::pair<typename X, typename Y>& v)
	{
		sprintf(to_string_buffer, "(Key: %s, Value: %s)", to_string(v.first), to_string(v.second));
		return to_string_buffer;
	}

	template<>
	static const char* to_string_internal<const char*>(const char* const& v, const std::false_type& ignored)
	{
		return v;
	}

	template <typename First>
	static void print_log_internal(char* buffer, int32& position, First&& first)
	{
		const char* formatted = sp::internal::to_string<First>(first);
		int32 length = strlen(formatted);
		memcpy(&buffer[position], formatted, length);
		position += length;
	}

	template <typename First, typename... Args>
	static void print_log_internal(char* buffer, int32& position, First&& first, Args&&... args)
	{
		const char* formatted = sp::internal::to_string<First>(first);
		int32 length = strlen(formatted);
		memcpy(&buffer[position], formatted, length);
		position += length;
		if (sizeof...(Args))
			print_log_internal(buffer, position, std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void log_message(int32 level, bool newline, Args... args)
	{
		char buffer[1024 * 10];
		int32 position = 0;
		print_log_internal(buffer, position, std::forward<Args>(args)...);

		if (newline)
			buffer[position++] = '\n';

		buffer[position] = 0;

		PlatformLogMessage(level, buffer);
	}
} }

// Windows (wingdi.h) defines SP_ERROR
#ifdef SP_ERROR
	#undef SP_ERROR
#endif

#ifndef SPARKY_LOG_LEVEL
#define SPARKY_LOG_LEVEL SPARKY_LOG_LEVEL_INFO
#endif

#if SPARKY_LOG_LEVEL >= SPARKY_LOG_LEVEL_FATAL
#define SP_FATAL(...) sp::internal::log_message(SPARKY_LOG_LEVEL_FATAL, true, "SPARKY:    ", __VA_ARGS__)
#define _SP_FATAL(...) sp::internal::log_message(SPARKY_LOG_LEVEL_FATAL, false, __VA_ARGS__)
#else
#define SP_FATAL(...)
#define _SP_FATAL(...)
#endif

#if SPARKY_LOG_LEVEL >= SPARKY_LOG_LEVEL_ERROR
#define SP_ERROR(...) sp::internal::log_message(SPARKY_LOG_LEVEL_ERROR, true, "SPARKY:    ", __VA_ARGS__)
#define _SP_ERROR(...) sp::internal::log_message(SPARKY_LOG_LEVEL_ERROR, false, __VA_ARGS__)
#else
#define SP_ERROR(...)
#define _SP_ERROR(...)
#endif

#if SPARKY_LOG_LEVEL >= SPARKY_LOG_LEVEL_WARN
#define SP_WARN(...) sp::internal::log_message(SPARKY_LOG_LEVEL_WARN, true, "SPARKY:    ", __VA_ARGS__)
#define _SP_WARN(...) sp::internal::log_message(SPARKY_LOG_LEVEL_WARN, false, __VA_ARGS__)
#else
#define SP_WARN(...)
#define _SP_WARN(...)
#endif

#if SPARKY_LOG_LEVEL >= SPARKY_LOG_LEVEL_INFO
#define SP_INFO(...) sp::internal::log_message(SPARKY_LOG_LEVEL_INFO, true, "SPARKY:    ", __VA_ARGS__)
#define _SP_INFO(...) sp::internal::log_message(SPARKY_LOG_LEVEL_INFO, false, __VA_ARGS__)
#else
#define SP_INFO(...)
#define _SP_INFO(...)
#endif

#ifdef SP_DEBUG
#define SP_ASSERT(x, ...) \
		if (!(x)) {\
			SP_FATAL("*************************"); \
			SP_FATAL("    ASSERTION FAILED!    "); \
			SP_FATAL("*************************"); \
			SP_FATAL(__FILE__, ": ", __LINE__); \
			SP_FATAL("Condition: ", #x); \
			SP_FATAL(__VA_ARGS__); \
			__debugbreak(); \
		}
#else
#define SP_ASSERT(x, ...)
#endif