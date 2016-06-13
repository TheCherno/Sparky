#pragma once

#ifndef LUA_H
#define LUA_H
#include <lua.hpp>
#include "luawrapper.h"
#include "luawrapperutils.h"
#endif

#include "Sparky.h"

using namespace sp;
using namespace graphics;
using namespace audio;
using namespace debug;
using namespace entity;
using namespace component;
using namespace internal;
using namespace API;
using namespace ui;
using namespace events;
using namespace maths;
using namespace ftgl;

namespace sp { namespace scripting {
	void Load(lua_State* L)	{
		luabind::module(L)
		[
			luabind::class_<StringFormat>("StringFormat"),

			luabind::class_<Members>("Members"),

			luabind::class_<SystemInfo>("SystemInfo"),

			luabind::class_<Quaternion>("Quaternion"),

			luabind::class_<Rectangle>("Rectangle"),

			luabind::class_<vec2>("vec2"),

			luabind::class_<mat4>("mat4"),

			luabind::class_<mat4>("mat4"),

			luabind::class_<texture_atlas_t>("texture_atlas_t"),

			luabind::class_<texture_font_t>("texture_font_t"),

			luabind::class_<Renderer3D>("Renderer3D"),

			luabind::class_<Renderer3D>("Renderer3D"),

			luabind::class_<Vertex>("Vertex"),

			luabind::class_<VertexData>("VertexData"),

			luabind::class_<RendererUniform>("RendererUniform"),

			luabind::class_<RenderCommand>("RenderCommand"),

			luabind::class_<Renderable2D>("Renderable2D"),

			luabind::class_<Renderer3D>("Renderer3D"),

			luabind::class_<Widget>("Widget"),

			luabind::class_<BufferElement>("BufferElement"),

			luabind::class_<Entity>("Entity"),

			luabind::class_<ComponentType>("ComponentType"),

			luabind::class_<IAction>("IAction"),

			luabind::class_<DebugMenuSettings>("DebugMenuSettings"),

			luabind::class_<IAction>("IAction"),

			luabind::class_<DebugMenuSlider>("DebugMenuSlider"),

			luabind::class_<ga_Sound>("ga_Sound"),

			luabind::class_<ga_Handle>("ga_Handle"),

			luabind::class_<gau_Manager>("gau_Manager"),

			luabind::class_<ga_Mixer>("ga_Mixer"),

			luabind::class_<DebugLayer>("DebugLayer"),

			luabind::class_<Window>("Window"),

			luabind::class_<WindowProperties>("WindowProperties")
		];
	}
} }