#include "sp/sp.h"
#include "AudioScript.h"

#include <lua.hpp>
#include <luawrapper.hpp>
#include <luawrapperutil.hpp>

#include "sp/maths/maths.h"

using namespace sp;
using namespace maths;

namespace sp { namespace scripting { namespace maths {

	vec2* L_Vec2_New(lua_State *L)
	{
		float x = lua_tonumber(L, 1);
		float y = lua_tonumber(L, 2);
		return new vec2(x, y);
	}

	vec3* L_Vec3_New(lua_State *L)
	{
		float x = lua_tonumber(L, 1);
		float y = lua_tonumber(L, 2);
		float z = lua_tonumber(L, 3);
		return new vec3(x, y, z);
	}

	vec4* L_Vec4_New(lua_State *L)
	{
		float x = lua_tonumber(L, 1);
		float y = lua_tonumber(L, 2);
		float z = lua_tonumber(L, 3);
		float w = lua_tonumber(L, 4);
		return new vec4(x, y, z, w);
	}

	Quaternion* L_Quaternion_New(lua_State *L)
	{
		float x = lua_tonumber(L, 1);
		float y = lua_tonumber(L, 2);
		float z = lua_tonumber(L, 3);
		float w = lua_tonumber(L, 4);
		return new Quaternion(x, y, z, w);
	}

	int L_Vec2_Add(lua_State *L) 
	{

		return 1;
	}

	int L_Vec2_Subtract(lua_State *L) 
	{
		return 1;
	}

	int L_Vec2_Multiply(lua_State *L) 
	{
		return 1;
	}

	int L_Vec2_Divide(lua_State *L) 
	{
		return 1;
	}

	int L_Vec2_GetX(lua_State *L)
	{
		return 1;
	}

	int L_Vec2_GetY(lua_State *L)
	{
		return 1;
	}

	int L_Vec2_SetX(lua_State *L)
	{
		return 1;
	}

	int L_Vec2_SetY(lua_State *L)
	{
		return 1;
	}

	luaL_Reg L_Vec2_Table[] = {
		{ NULL, NULL }
	};

	luaL_Reg L_Vec2_MetaTable[] = {
		{ "Add", L_Vec2_Add },
		{ "Sub", L_Vec2_Subtract },
		{ "Mul", L_Vec2_Multiply },
		{ "Div", L_Vec2_Divide },
		{ "Subtract", L_Vec2_Subtract },
		{ "Multiply", L_Vec2_Multiply },
		{ "Divide", L_Vec2_Divide },
		{ NULL, NULL }
	};

	luaL_Reg L_Vec3_Table[] = {
		{ NULL, NULL }
	};

	luaL_Reg L_Vec3_MetaTable[] = {
		{ NULL, NULL }
	};

	luaL_Reg L_Vec4_Table[] = {
		{ NULL, NULL }
	};

	luaL_Reg L_Vec4_MetaTable[] = {
		{ NULL, NULL }
	};

	luaL_Reg L_Quaternion_Table[] = {
		{ NULL, NULL }
	};

	luaL_Reg L_Quaternion_MetaTable[] = {
		{ NULL, NULL }
	};
	
	int L_Register_Math_Classes(lua_State *L) {
		luaW_register<vec2>(L, "vec2", L_Vec2_Table, L_Vec2_MetaTable, L_Vec2_New);
		luaW_register<vec3>(L, "vec3", L_Vec3_Table, L_Vec3_MetaTable, L_Vec3_New);
		luaW_register<vec4>(L, "vec4", L_Vec4_Table, L_Vec4_MetaTable, L_Vec4_New);
		luaW_register<Quaternion>(L, "Quaternion", L_Quaternion_Table, L_Quaternion_MetaTable, L_Quaternion_New);
		return 1;
	}

} } }
