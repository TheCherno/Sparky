#pragma once

struct lua_State;
typedef int(*lua_CFunction) (lua_State *L);

namespace sp { namespace scripting {

	void PushValue(lua_State *vm, lua_Integer n);
	void PushValue(lua_State *vm, lua_Number n);
	void PushValue(lua_State *vm, bool b);
	void PushValue(lua_State *vm, const String& s);

	template <typename T>
	struct value_extractor;

	template <>
	struct value_extractor<lua_Integer>;

	template <>
	struct value_extractor<lua_Number>;

	template <>
	struct value_extractor<bool>;

	template <>
	struct value_extractor<std::string>;

} }