#include "sp/sp.h"
#include "script.h"

namespace sp { namespace scripting {

	Script::Script()
	{
		m_Files.push_back("");
	}

	void Script::CreateState()
	{
		Create();
	}

	void Script::AddFile(const char* file) {
		m_Files.push_back(file);
	}

	void Script::Create() {
		m_State = luaL_newstate();


		luaopen_io(m_State);
		luaopen_base(m_State);
		luaopen_table(m_State);
		luaopen_string(m_State);
		luaopen_math(m_State);
		luaL_openlibs(m_State);
		luabind::open(m_State);

		sp::scripting::Load(m_State);

		for (int i = 0; i < m_Files.size(); i++) {
			LoadLuaFile(m_State, m_Files[i]);
		}
	}

	void Script::LoadLuaFile(lua_State* L, const char* file)
	{
		String path = file;
		//if (VFS::Get()->ResolvePhysicalPath(file, path)) {
		luaL_loadfile(L, path.c_str());
		int errors = lua_pcall(L, 0, LUA_MULTRET, 0);
		if (errors != 0)
		{
			std::string s(lua_tostring(L, -1));
			SP_ERROR("Lua >> ", s);
			lua_pop(L, 1);
		}
		/*}
		else {
		SP_ERROR("Lua >> Could not find file in the VFS!");
		}*/
	}

} }