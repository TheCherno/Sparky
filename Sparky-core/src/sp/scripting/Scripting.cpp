#include "sp/sp.h"
#include "Scripting.h"

#include <lua.hpp>

#include "sp/utils/Log.h"
#include "sp/system/VFS.h"

#include "AudioScript.h"
#include "ScriptFunction.h"

namespace sp { namespace scripting {

	lua_State* Scripting::CreateState(lua_State** state)
	{
		lua_State* s = luaL_newstate();
		*state = s;
		return s;
	}

	void Scripting::Init(lua_State* state)
	{
		luaopen_io(state);
		luaopen_base(state);
		luaopen_table(state);
		luaopen_string(state);
		luaopen_math(state);

		luaL_openlibs(state);
	}

	void Scripting::LoadFile(lua_State* state, const char* filename)
	{
		String physicalPath;
		if (VFS::Get()->ResolvePhysicalPath(filename, physicalPath))
		{
			luaL_loadfile(state, physicalPath.c_str());
			int errors = lua_pcall(state, 0, LUA_MULTRET, 0);
		}
		else {
			SP_ERROR("LUA: Could not find file!", physicalPath.c_str());
		}
	}

	void Scripting::Register(lua_State* state, const char* functionname, lua_CFunction functionpointer)
	{
		lua_register(state, functionname, functionpointer);
	}

	bool Scripting::Call(lua_State* state, const char* functionname)
	{
		lua_getglobal(state, functionname);
		if (!lua_isfunction(state, -1)) return true;

		lua_pcall(state, 0, 0, 0);
		return false;
	}

	void Scripting::LoadSparkyAPI(lua_State* state)
	{
		audio::L_Register_Audio_Classes(state);
	}


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

	class lua_function_base
	{
	public:
		lua_function_base(lua_State *vm, const std::string& func)
			: m_vm(vm)
		{
			// get the function
			lua_getfield(m_vm, LUA_GLOBALSINDEX, func.c_str());
			// ensure it's a function
			if (!lua_isfunction(m_vm, -1)) {
				// throw an exception; you'd use your own exception class here
				// of course, but for sake of simplicity i use runtime_error
				lua_pop(m_vm, 1);
				throw std::runtime_error("not a valid function");
			}
			// store it in registry for later use
			m_func = luaL_ref(m_vm, LUA_REGISTRYINDEX);
		}

		lua_function_base(const lua_function_base& func)
			: m_vm(func.m_vm)
		{
			// copy the registry reference
			lua_rawgeti(m_vm, LUA_REGISTRYINDEX, func.m_func);
			m_func = luaL_ref(m_vm, LUA_REGISTRYINDEX);
		}

		~lua_function_base()
		{
			// delete the reference from registry
			luaL_unref(m_vm, LUA_REGISTRYINDEX, m_func);
		}

		lua_function_base& operator=(const lua_function_base& func)
		{
			if (this != &func) {
				m_vm = func.m_vm;
				lua_rawgeti(m_vm, LUA_REGISTRYINDEX, func.m_func);
				m_func = luaL_ref(m_vm, LUA_REGISTRYINDEX);
			}
			return *this;
		}
	private:
		// the virtual machine and the registry reference to the function
		lua_State *m_vm;
		int m_func;

		// call the function, throws an exception on error
		void call(int args, int results)
		{
			// call it with no return values
			int status = lua_pcall(m_vm, args, results, 0);
			if (status != 0) {
				// call failed; throw an exception
				std::string error = lua_tostring(m_vm, -1);
				lua_pop(m_vm, 1);
				// in reality you'd want to use your own exception class here
				throw std::runtime_error(error.c_str());
			}
		}
	};

	// the function wrapper class
	template <typename Ret>
	class lua_function : public lua_function_base
	{
	public:
		lua_function(lua_State *vm, const std::string& func)
			: lua_function_base(vm, func)
		{
		}

		Ret operator()()
		{
			// push the function from the registry
			lua_rawgeti(m_vm, LUA_REGISTRYINDEX, m_func);
			// call the function on top of the stack (throws exception on error)
			call(0);
			// return the value
			return detail::value_extractor<Ret>::get(m_vm);
		}

		template <typename T1>
		Ret operator()(const T1& p1)
		{
			lua_rawgeti(m_vm, LUA_REGISTRYINDEX, m_func);
			// push the argument and call with 1 arg
			detail::push_value(m_vm, p1);
			call(1);
			return detail::value_extractor<Ret>::get(m_vm);
		}

		template <typename T1, typename T2>
		Ret operator()(const T1& p1, const T2& p2)
		{
			lua_rawgeti(m_vm, LUA_REGISTRYINDEX, m_func);
			// push the arguments and call with 2 args
			detail::push_value(m_vm, p1);
			detail::push_value(m_vm, p2);
			call(2);
			return detail::value_extractor<Ret>::get(m_vm);
		}

		template <typename T1, typename T2, typename T3>
		Ret operator()(const T1& p1, const T2& p2, const T3& p3)
		{
			lua_rawgeti(m_vm, LUA_REGISTRYINDEX, m_func);
			detail::push_value(m_vm, p1);
			detail::push_value(m_vm, p2);
			detail::push_value(m_vm, p3);
			call(3);
			return detail::value_extractor<Ret>::get(m_vm);
		}

		// et cetera, provide as many overloads as you need
	};

	// we need to specialize the function for void return type
	// as the other class would fail to compile with void as return type
	template <>
	class lua_function<void> : public lua_function_base
	{
	public:
		lua_function(lua_State *vm, const std::string& func)
			: lua_function_base(vm, func)
		{
		}

		void operator()()
		{
			lua_rawgeti(m_vm, LUA_REGISTRYINDEX, m_func);
			call(0);
		}

		template <typename T1>
		void operator()(const T1& p1)
		{
			lua_rawgeti(m_vm, LUA_REGISTRYINDEX, m_func);
			detail::push_value(m_vm, p1);
			call(1);
		}

		template <typename T1, typename T2>
		void operator()(const T1& p1, const T2& p2)
		{
			lua_rawgeti(m_vm, LUA_REGISTRYINDEX, m_func);
			detail::push_value(m_vm, p1);
			detail::push_value(m_vm, p2);
			call(2);
		}

		template <typename T1, typename T2, typename T3>
		void operator()(const T1& p1, const T2& p2, const T3& p3)
		{
			lua_rawgeti(m_vm, LUA_REGISTRYINDEX, m_func);
			detail::push_value(m_vm, p1);
			detail::push_value(m_vm, p2);
			detail::push_value(m_vm, p3);
			call(3);
		}

		// et cetera, provide as many overloads as you need
	};
} }