#pragma once

#include <string>
#include <unordered_map>

typedef unsigned char byte;
typedef unsigned int uint;
typedef std::string String;

#define BIT(x) (1 << x)

struct vec4
{
	float x, y, z, w;

	vec4()
		: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{}
	vec4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{}

	bool operator==(const vec4& other) const
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool operator!=(const vec4& other) const
	{
		return !(*this == other);
	}

	uint GetHash() const
	{
		return (*(uint*)&x) ^ ((*(uint*)&y) << 14) ^ ((*(uint*)&z) << 23) ^ ((*(uint*)&w) << 31);
	}
};

struct vec3
{
	float x, y, z;

	vec3()
		: x(0.0f), y(0.0f), z(0.0f)
	{}
	vec3(float x, float y, float z)
		: x(x), y(y), z(z)
	{}

	bool operator==(const vec3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const vec3& other) const
	{
		return !(*this == other);
	}

	uint GetHash() const
	{
		return (*(uint*)&x) ^ ((*(uint*)&y) << 14) ^ ((*(uint*)&z) << 23);
	}

};

struct vec2
{
	float x, y;

	vec2()
		: x(0.0f), y(0.0f)
	{}
	vec2(float x, float y)
		: x(x), y(y)
	{}

	bool operator==(const vec2& other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const vec2& other) const
	{
		return !(*this == other);
	}

	uint GetHash() const
	{
		return (*(uint*)&x) ^ ((*(uint*)&y) << 14);
	}

};

struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 uv;
	vec3 binormal;
	vec3 tangent;

	bool operator==(const Vertex& other) const
	{
		return position == other.position && normal == other.normal && uv == other.uv && binormal == other.binormal && tangent == other.tangent;
	}
};

template<>
struct std::hash<Vertex>
{
	const size_t operator()(const Vertex& key) const
	{
		return key.position.GetHash() ^ key.normal.GetHash() ^ key.uv.GetHash() ^ key.binormal.GetHash() ^ key.tangent.GetHash();
	}
};

