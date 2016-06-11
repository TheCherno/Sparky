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

namespace sp { namespace scripting {
	void Load(lua_State* L)	{
		_SP_CLASSREGISTER(Timer);
		_SP_CLASSREGISTER(FileSystem);
		_SP_CLASSREGISTER(MemoryManager);
		SP_CLASSREGISTER(BatchRenderer2D, uint, uint);
		SP_CLASSREGISTER(DeferredRenderer, uint, uint);
		SP_CLASSREGISTER(Font, const String &, const String &, float);
		_SP_CLASSREGISTER(ForwardRenderer);
		SP_CLASSREGISTER(GBuffer, uint, uint);
		SP_CLASSREGISTER(Label, const String &, float, float, uint);
		_SP_CLASSREGISTER(LightSetup);
		SP_CLASSREGISTER(Material, API::Shader *);
		SP_CLASSREGISTER(MaterialInstance, sp::graphics::Material *);
		SP_CLASSREGISTER(Mesh, API::VertexArray *, API::IndexBuffer *, sp::graphics::MaterialInstance *);
		SP_CLASSREGISTER(Model, const String &, sp::graphics::MaterialInstance *);
		SP_CLASSREGISTER(PBRMaterial, API::Shader *);
		SP_CLASSREGISTER(PBRMaterialInstance, sp::graphics::PBRMaterial *);
		SP_CLASSREGISTER(Renderable2D, const maths::vec3 &, const maths::vec2 &, uint);
		_SP_CLASSREGISTER(Scene);
		SP_CLASSREGISTER(Sprite, API::Texture2D *);
		SP_CLASSREGISTER(Button, const String &, const maths::Rectangle &, const int &);
		_SP_CLASSREGISTER(Panel);
		SP_CLASSREGISTER(Slider, const maths::Rectangle &, bool);
		SP_CLASSREGISTER(ShaderStruct, const String &);
		_SP_CLASSREGISTER(PostEffects);
		SP_CLASSREGISTER(PostEffectsPass, API::Shader *);
		SP_CLASSREGISTER(Group, const maths::mat4 &);
		_SP_CLASSREGISTER(Layer);
		SP_CLASSREGISTER(Layer2D, const maths::mat4 &);
		SP_CLASSREGISTER(Layer3D, sp::graphics::Scene *, sp::graphics::Renderer3D *);
		SP_CLASSREGISTER(Camera, const maths::mat4 &);
		SP_CLASSREGISTER(FPSCamera, const maths::mat4 &);
		SP_CLASSREGISTER(MayaCamera, const maths::mat4 &);
		SP_CLASSREGISTER(BufferLayout, const String &, uint, bool);
		_SP_CLASSREGISTER(IndexBuffer);
		SP_CLASSREGISTER(KeyEvent, int32, Event::Type);
		SP_CLASSREGISTER(KeyPressedEvent, int32, int32, int32);
		SP_CLASSREGISTER(KeyReleasedEvent, int32);
		SP_CLASSREGISTER(MousePressedEvent, int32, float, float);
		SP_CLASSREGISTER(MouseReleasedEvent, int32, float, float);
		SP_CLASSREGISTER(MouseMovedEvent, float, float, bool);
		SP_CLASSREGISTER(ResizeWindowEvent, uint, uint);
		_SP_CLASSREGISTER(Entity);
		SP_CLASSREGISTER(MeshComponent, graphics::Mesh *);
		SP_CLASSREGISTER(TransformComponent, const maths::mat4 &);
		_SP_CLASSREGISTER(DebugLayer);
		SP_CLASSREGISTER(DebugMenu, const String &);
		SP_CLASSREGISTER(DebugMenuItem, sp::debug::IAction *, const maths::Rectangle &);
		SP_CLASSREGISTER(Sound, const String &, const String &);
		SP_CLASSREGISTER(Application, const String &, const sp::WindowProperties &, graphics::API::RenderAPI);
		_SP_CLASSREGISTER(InputManager);
		SP_CLASSREGISTER(Window, const String &, const sp::WindowProperties &);
}
}
}
