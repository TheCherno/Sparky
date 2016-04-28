#include "sp/sp.h"
#include "ScriptFunction.h"

#include <lua.hpp>

#include "sp/utils/Log.h"

using namespace sp;

namespace sp { namespace scripting {

	void PushValue(lua_State *vm, lua_Integer n)
	{
		lua_pushinteger(vm, n);
	}

	void PushValue(lua_State *vm, lua_Number n)
	{
		lua_pushnumber(vm, n);
	}

	void PushValue(lua_State *vm, bool b)
	{
		lua_pushboolean(vm, b);
	}

	void PushValue(lua_State *vm, const String& s)
	{
		lua_pushstring(vm, s.c_str());
	}

	template <typename T>
	struct value_extractor
	{
	};

	template <>
	struct value_extractor<lua_Integer>
	{
		static lua_Integer get(lua_State *vm)
		{
			lua_Integer val = lua_tointeger(vm, -1);
			lua_pop(vm, 1);
			return val;
		}
	};

	template <>
	struct value_extractor<lua_Number>
	{
		static lua_Number get(lua_State *vm)
		{
			lua_Number val = lua_tonumber(vm, -1);
			lua_pop(vm, 1);
			return val;
		}
	};

	template <>
	struct value_extractor<bool>
	{
		static bool get(lua_State *vm)
		{
			bool val = lua_toboolean(vm, -1);
			lua_pop(vm, 1);
			return val;
		}
	};

	template <>
	struct value_extractor<std::string>
	{
		static std::string get(lua_State *vm)
		{
			std::string val = lua_tostring(vm, -1);
			lua_pop(vm, 1);
			return val;
		}
	};

} }