#include "sp/sp.h"
#include "AudioScript.h"

#include <lua.hpp>
#include <luawrapper.hpp>
#include <luawrapperutil.hpp>

#include "sp/utils/Log.h"
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
		vec2* a = luaW_check<vec2>(L, 1);
		vec2* b = luaW_check<vec2>(L, 2);
		*a += *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec2_Subtract(lua_State *L) 
	{
		vec2* a = luaW_check<vec2>(L, 1);
		vec2* b = luaW_check<vec2>(L, 2);
		*a -= *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec2_Multiply(lua_State *L) 
	{
		vec2* a = luaW_check<vec2>(L, 1);
		vec2* b = luaW_check<vec2>(L, 2);
		*a *= *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec2_Divide(lua_State *L) 
	{
		vec2* a = luaW_check<vec2>(L, 1);
		vec2* b = luaW_check<vec2>(L, 2);
		*a /= *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec2_GetX(lua_State *L)
	{
		vec2* v = luaW_check<vec2>(L, 1);
		lua_pushnumber(L, v->x);
		return 1;
	}

	int L_Vec2_GetY(lua_State *L)
	{
		vec2* v = luaW_check<vec2>(L, 1);
		lua_pushnumber(L, v->y);
		return 1;
	}

	int L_Vec2_SetX(lua_State *L)
	{
		vec2* v = luaW_check<vec2>(L, 1);
		float x = lua_tonumber(L, 2);
		v->x = x;
		return 0;
	}

	int L_Vec2_SetY(lua_State *L)
	{
		vec2* v = luaW_check<vec2>(L, 1);
		float y = lua_tonumber(L, 2);
		v->y = y;
		return 0;
	}

	int L_Vec3_Add(lua_State *L)
	{
		vec3* a = luaW_check<vec3>(L, 1);
		vec3* b = luaW_check<vec3>(L, 2);
		*a += *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec3_Subtract(lua_State *L)
	{
		vec3* a = luaW_check<vec3>(L, 1);
		vec3* b = luaW_check<vec3>(L, 2);
		*a -= *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec3_Multiply(lua_State *L)
	{
		vec3* a = luaW_check<vec3>(L, 1);
		vec3* b = luaW_check<vec3>(L, 2);
		*a *= *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec3_Divide(lua_State *L)
	{
		vec3* a = luaW_check<vec3>(L, 1);
		vec3* b = luaW_check<vec3>(L, 2);
		*a /= *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec3_GetX(lua_State *L)
	{
		vec3* v = luaW_check<vec3>(L, 1);
		lua_pushnumber(L, v->x);
		return 1;
	}

	int L_Vec3_GetY(lua_State *L)
	{
		vec3* v = luaW_check<vec3>(L, 1);
		lua_pushnumber(L, v->y);
		return 1;
	}

	int L_Vec3_GetZ(lua_State *L)
	{
		vec3* v = luaW_check<vec3>(L, 1);
		lua_pushnumber(L, v->z);
		return 1;
	}

	int L_Vec3_SetX(lua_State *L)
	{
		vec3* v = luaW_check<vec3>(L, 1);
		float x = lua_tonumber(L, 2);
		v->x = x;
		return 0;
	}

	int L_Vec3_SetY(lua_State *L)
	{
		vec3* v = luaW_check<vec3>(L, 1);
		float y = lua_tonumber(L, 2);
		v->y = y;
		return 0;
	}

	int L_Vec3_SetZ(lua_State *L)
	{
		vec3* v = luaW_check<vec3>(L, 1);
		float z = lua_tonumber(L, 2);
		v->z = z;
		return 0;
	}

	int L_Vec4_Add(lua_State *L)
	{
		vec4* a = luaW_check<vec4>(L, 1);
		vec4* b = luaW_check<vec4>(L, 2);
		*a += *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec4_Subtract(lua_State *L)
	{
		vec4* a = luaW_check<vec4>(L, 1);
		vec4* b = luaW_check<vec4>(L, 2);
		*a -= *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec4_Multiply(lua_State *L)
	{
		vec4* a = luaW_check<vec4>(L, 1);
		vec4* b = luaW_check<vec4>(L, 2);
		*a *= *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec4_Divide(lua_State *L)
	{
		vec4* a = luaW_check<vec4>(L, 1);
		vec4* b = luaW_check<vec4>(L, 2);
		*a /= *b;
		lua_pop(L, 1);
		return 1;
	}

	int L_Vec4_GetX(lua_State *L)
	{
		vec4* v = luaW_check<vec4>(L, 1);
		lua_pushnumber(L, v->x);
		return 1;
	}

	int L_Vec4_GetY(lua_State *L)
	{
		vec4* v = luaW_check<vec4>(L, 1);
		lua_pushnumber(L, v->y);
		return 1;
	}

	int L_Vec4_GetZ(lua_State *L)
	{
		vec4* v = luaW_check<vec4>(L, 1);
		lua_pushnumber(L, v->z);
		return 1;
	}

	int L_Vec4_GetW(lua_State *L)
	{
		vec4* v = luaW_check<vec4>(L, 1);
		lua_pushnumber(L, v->w);
		return 1;
	}

	int L_Vec4_SetX(lua_State *L)
	{
		vec4* v = luaW_check<vec4>(L, 1);
		float x = lua_tonumber(L, 2);
		v->x = x;
		return 0;
	}

	int L_Vec4_SetY(lua_State *L)
	{
		vec4* v = luaW_check<vec4>(L, 1);
		float y = lua_tonumber(L, 2);
		v->y = y;
		return 0;
	}

	int L_Vec4_SetZ(lua_State *L)
	{
		vec4* v = luaW_check<vec4>(L, 1);
		float z = lua_tonumber(L, 2);
		v->z = z;
		return 0;
	}

	int L_Vec4_SetW(lua_State *L)
	{
		vec4* v = luaW_check<vec4>(L, 1);
		float w = lua_tonumber(L, 2);
		v->w = w;
		return 0;
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
		{ "GetX", L_Vec2_GetX },
		{ "GetY", L_Vec2_GetY },
		{ "SetX", L_Vec2_SetX },
		{ "SetY", L_Vec2_SetY },
		{ NULL, NULL }
	};

	luaL_Reg L_Vec3_Table[] = {
		{ NULL, NULL }
	};

	luaL_Reg L_Vec3_MetaTable[] = {
		{ "Add", L_Vec3_Add },
		{ "Sub", L_Vec3_Subtract },
		{ "Mul", L_Vec3_Multiply },
		{ "Div", L_Vec3_Divide },
		{ "Subtract", L_Vec3_Subtract },
		{ "Multiply", L_Vec3_Multiply },
		{ "Divide", L_Vec3_Divide },
		{ "GetX", L_Vec3_GetX },
		{ "GetY", L_Vec3_GetY },
		{ "GetZ", L_Vec3_GetZ },
		{ "SetX", L_Vec3_SetX },
		{ "SetY", L_Vec3_SetY },
		{ "SetZ", L_Vec3_SetZ },
		{ NULL, NULL }
	};

	luaL_Reg L_Vec4_Table[] = {
		{ NULL, NULL }
	};

	luaL_Reg L_Vec4_MetaTable[] = {
		{ "Add", L_Vec4_Add },
		{ "Sub", L_Vec4_Subtract },
		{ "Mul", L_Vec4_Multiply },
		{ "Div", L_Vec4_Divide },
		{ "Subtract", L_Vec4_Subtract },
		{ "Multiply", L_Vec4_Multiply },
		{ "Divide", L_Vec4_Divide },
		{ "GetX", L_Vec4_GetX },
		{ "GetY", L_Vec4_GetY },
		{ "GetZ", L_Vec4_GetZ },
		{ "GetW", L_Vec4_GetW },
		{ "SetX", L_Vec4_SetX },
		{ "SetY", L_Vec4_SetY },
		{ "SetZ", L_Vec4_SetZ },
		{ "SetW", L_Vec4_SetW },
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
