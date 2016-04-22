#pragma once

struct lua_State;

int CreateSoundClass(lua_State *L);

int AddSound(lua_State *L);
int GetSound(lua_State *L);

int PlaySound(lua_State *L);
int LoopSound(lua_State *L);