#pragma once

#ifndef SCRIPT_H
#define SCRIPT_H

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luabind\luabind.hpp"
#endif

#include "API.h"
#include "marshal.h"
#include "unmarshal.h"
#include "luaconstructor.h"
#include "luacall.h"
#include "luafunction.h"

namespace sp { namespace scripting {

	class SP_API Script {
	private:
		lua_State* m_State;
		std::vector<const char*> m_Files;
	public:
		Script();

		void CreateState();
		void AddFile(const char* file);

		template <typename... Args>
		void CallFunction(const char* func, Args&&... args) {
			sp::scripting::FunctionCaller::Dispatch(m_State, func, args...);
		}
	private:
		void Create();

		void LoadLuaFile(lua_State* L, const char* file);
	};
} }

#endif