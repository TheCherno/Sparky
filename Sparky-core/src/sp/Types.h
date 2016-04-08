#pragma once

typedef int8_t		int8;
typedef int16_t		int16;
typedef int32_t		int32;
typedef int64_t		int64;

typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef uint64_t	uint64;

typedef uint8	byte;
typedef uint32	uint;

typedef std::string String;

// Sparky Class Forward-Declarations
namespace sp {

	namespace graphics {
	}

	namespace events {
		class Event;
		class KeyPressedEvent;
		class KeyReleasedEvent;
		class MousePressedEvent;
		class MouseReleasedEvent;
		class MouseMovedEvent;
	}

	namespace maths {
		struct vec2;
		struct vec3;
		struct vec4;
		struct mat4;
		struct AABB;
		struct Rectangle;
	}
}

