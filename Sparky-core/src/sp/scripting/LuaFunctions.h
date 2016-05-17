#pragma once

#include "sp/sp.h"

#include <string>

#include <luabind\luabind.hpp>

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

typedef double lua_Number;
typedef ptrdiff_t lua_Integer;

namespace sp { namespace scripting {

	class SP_API LuaFunctions
	{
	private:

		template< typename PolicyList, unsigned int Pos >
		static void PushArguments(lua_State* L, int32& position) {};

		template< typename PolicyList, unsigned int Pos, typename Arg0, typename... Args >
		static void PushArguments(lua_State* L, int32& position, Arg0&& arg0, Args&&... args)
		{
			position++;
			using converter_type = luabind::detail::specialized_converter_policy< luabind::detail::fetched_converter_policy<Pos, PolicyList>, Arg0, luabind::detail::cpp_to_lua >;
			converter_type().to_lua(L, luabind::detail::unwrapped<Arg0>::get(std::forward<Arg0>(arg0)));
			PushArguments<PolicyList, Pos + 1>(L, position, std::forward<Args>(args)...);
		}

		static void CallPreInternal(lua_State* state, const char* functionname);
		static void CallAfterInternal(lua_State* state, int nargs);
	public:
		static bool Call(lua_State* state, const char* functionname);

		template <typename T, typename... Args>
		static void Call(lua_State* state, const char* functionname, Args&&... args)
		{
			//luabind::call_function<T>(state, functionname, std::forward<Args>(args)...);

			int32 position = 0;
			CallPreInternal(state, functionname);
			PushArguments<luabind::no_policies, 1>(state, position, std::forward<Args>(args)...);
			CallAfterInternal(state, position);
			SP_INFO(position);
		}
	};

} }