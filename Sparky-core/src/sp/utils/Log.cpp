#include "sp/sp.h"
#include "Log.h"

#include "sp/maths/vec2.h"
#include "sp/maths/Rectangle.h"
#include "sp/events/Events.h"

namespace sp { namespace internal {

	template <>
	const char* to_string<char>(char const& t)
	{
		return &t;
	}

	template <>
	const char* to_string<char*>(char* const& t)
	{
		return t;
	}

	template <>
	const char* to_string<unsigned char const*>(unsigned char const* const& t)
	{
		return (const char*)t;
	}

	template <>
	const char* to_string<char const*>(char const* const& t)
	{
		return t;
	}

	template <>
	const char* to_string<String>(const String& t)
	{
		return t.c_str();
	}

	template <>
	const char* to_string<bool>(const bool& t)
	{
		return t ? "true" : "false";
	}

	template <>
	const char* to_string<maths::vec2>(const maths::vec2& t)
	{
		// TODO: sprintf
		String string = String("vec2: (") + StringFormat::ToString(t.x) + ", " + StringFormat::ToString(t.y) + ")";
		char* result = new char[string.length()];
		strcpy(result, &string[0]);
		return result;
	}

	template <>
	const char* to_string<maths::vec3>(const maths::vec3& t)
	{
		// TODO: sprintf
		String string = String("vec3: (") + StringFormat::ToString(t.x) + ", " + StringFormat::ToString(t.y) + ", " + StringFormat::ToString(t.z) + ")";
		char* result = new char[string.length()];
		strcpy(result, &string[0]);
		return result;
	}

	template <>
	const char* to_string<maths::Rectangle>(const maths::Rectangle& r)
	{
		sprintf(sprintf_buffer, "Rectangle: (%f, %f, %f, %f)", r.x, r.y, r.width, r.height);
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	const char* to_string<events::KeyPressedEvent>(const events::KeyPressedEvent& e)
	{
		sprintf(sprintf_buffer, "KeyPressedEvent: (%d, %d)", e.GetKeyCode(), e.GetRepeat());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	const char* to_string<events::KeyReleasedEvent>(const events::KeyReleasedEvent& e)
	{
		sprintf(sprintf_buffer, "KeyReleasedEvent: (%d)", e.GetKeyCode());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	const char* to_string<events::MousePressedEvent>(const events::MousePressedEvent& e)
	{
		sprintf(sprintf_buffer, "MousePressedEvent: (%d, %f, %f)", e.GetButton(), e.GetX(), e.GetY());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	const char* to_string<events::MouseReleasedEvent>(const events::MouseReleasedEvent& e)
	{
		sprintf(sprintf_buffer, "MouseReleasedEvent: (%d, %f, %f)", e.GetButton(), e.GetX(), e.GetY());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	const char* to_string<events::MouseMovedEvent>(const events::MouseMovedEvent& e)
	{
		sprintf(sprintf_buffer, "MouseMovedEvent: (%f, %f, %s)", e.GetX(), e.GetY(), e.IsDragged() ? "true" : "false");
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	const char* to_string<events::Event>(const events::Event& e)
	{
		sprintf(sprintf_buffer, "Event: %s (%d)", events::Event::TypeToString(e.GetType()).c_str(), e.GetType());
		char* result = new char[strlen(sprintf_buffer)];
		strcpy(result, &sprintf_buffer[0]);
		return result;
	}

	template <>
	const char* to_string<events::Event*>(events::Event* const& e)
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

	template<>
	const char* to_string_internal<const char*>(const char* const& v, const std::false_type& ignored)
	{
		return v;
	}

} }