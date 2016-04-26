#pragma once

struct lua_State;
struct luaL_Reg;

namespace sp {
	namespace maths {
		class vec2;
		class vec3;
		class vec4;
		class Quaternion;
	}
}

namespace sp { namespace scripting { namespace maths {

	int L_Vec2_Add(lua_State *L);
	int L_Vec2_Subtract(lua_State *L);
	int L_Vec2_Multiply(lua_State *L);
	int L_Vec2_Divide(lua_State *L);

	int L_Vec2_GetX(lua_State *L);
	int L_Vec2_GetY(lua_State *L);

	int L_Vec2_SetX(lua_State *L);
	int L_Vec2_SetY(lua_State *L);

	int L_Vec3_Add(lua_State *L);
	int L_Vec3_Subtract(lua_State *L);
	int L_Vec3_Multiply(lua_State *L);
	int L_Vec3_Divide(lua_State *L);

	int L_Vec3_GetX(lua_State *L);
	int L_Vec3_GetY(lua_State *L);
	int L_Vec3_GetZ(lua_State *L);

	int L_Vec3_SetX(lua_State *L);
	int L_Vec3_SetY(lua_State *L);
	int L_Vec3_SetZ(lua_State *L);

	int L_Vec4_Add(lua_State *L);
	int L_Vec4_Subtract(lua_State *L);
	int L_Vec4_Multiply(lua_State *L);
	int L_Vec4_Divide(lua_State *L);

	int L_Vec4_GetX(lua_State *L);
	int L_Vec4_GetY(lua_State *L);
	int L_Vec4_GetZ(lua_State *L);
	int L_Vec4_GetW(lua_State *L);

	int L_Vec4_SetX(lua_State *L);
	int L_Vec4_SetY(lua_State *L);
	int L_Vec4_SetZ(lua_State *L);
	int L_Vec4_SetW(lua_State *L);

	sp::maths::vec2* L_Vec2_New(lua_State *L);
	sp::maths::vec3* L_Vec3_New(lua_State *L);
	sp::maths::vec4* L_Vec4_New(lua_State *L);
	sp::maths::Quaternion* L_Quaternion_New(lua_State *L);

	int L_Register_Math_Classes(lua_State *L);

} } }
