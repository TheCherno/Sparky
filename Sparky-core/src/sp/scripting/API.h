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
		_SP_CLASSREGISTER(StringFormat);
		_SP_CLASSREGISTER(Members);
		SP_FUNCTION(Timer, Reset, void);
		SP_FUNCTION(Timer, Elapsed, float);
		SP_FUNCTION(Timer, ElapsedMillis, float);
		_SP_CLASSREGISTER(Allocator);
		_SP_CLASSREGISTER(FileSystem);
		SP_FUNCTION(SystemMemoryInfo, Log, void);
		_SP_CLASSREGISTER(SystemMemoryInfo);
		_SP_CLASSREGISTER(SystemInfo);
		_SP_CLASSREGISTER(System);
		_SP_CLASSREGISTER(Rectangle);
		_SP_CLASSREGISTER(mat4);
		SP_FUNCTION(BatchRenderer2D, DrawString, void, const String &, const maths::vec2 &, const sp::graphics::Font &, uint);
		SP_CLASSREGISTER(BatchRenderer2D, uint, uint);
		SP_FUNCTION(DeferredRenderer, SubmitMesh, void, sp::graphics::Mesh *, const maths::mat4 &);
		SP_CLASSREGISTER(DeferredRenderer, uint, uint);
		_SP_CLASSREGISTER(texture_atlas_t);
		_SP_CLASSREGISTER(texture_font_t);
		SP_CLASSREGISTER(Font, const String &, const String &, float);
		_SP_CLASSREGISTER(FontManager);
		SP_FUNCTION(ForwardRenderer, SubmitMesh, void, sp::graphics::Mesh *, const maths::mat4 &);
		SP_CLASSREGISTER(GBuffer, uint, uint);
		_SP_CLASSREGISTER(Renderer3D);
		SP_FUNCTION(IRenderable, Render, void, sp::graphics::Renderer3D &);
		_SP_CLASSREGISTER(IRenderable);
		SP_CLASSREGISTER(Label, const String &, float, float, uint);
		SP_CLASSREGISTER(Light, const maths::vec3 &, float, const maths::vec4 &);
		SP_FUNCTION(LightSetup, Add, sp::graphics::Light *, sp::graphics::Light *);
		SP_FUNCTION(LightSetup, Remove, void, sp::graphics::Light *);
		SP_CLASSREGISTER(Mask, API::Texture2D *, const maths::mat4 &);
		SP_FUNCTION(Material, SetUniformData, void, const String &, byte *);
		SP_FUNCTION(Material, SetTexture, void, const String &, API::Texture *);
		SP_FUNCTION(Material, SetRenderFlags, void, int);
		SP_FUNCTION(Material, SetRenderFlag, void, Material::RenderFlags);
		SP_FUNCTION(Material, GetShader, API::Shader *, const String &, const T &, const String &, const API::ShaderUniformDeclaration *);
		SP_CLASSREGISTER(Material, API::Shader *);
		SP_FUNCTION(MaterialInstance, SetUniformData, void, const String &, byte *);
		SP_FUNCTION(MaterialInstance, SetTexture, void, const String &, API::Texture *);
		SP_FUNCTION(MaterialInstance, SetRenderFlag, void, Material::RenderFlags, const String &, const T &, const String &);
		_SP_CLASSREGISTER(Renderer3D);
		_SP_CLASSREGISTER(Vertex);
		SP_FUNCTION(Mesh, Render, void, sp::graphics::Renderer3D &, float, float, float, float, sp::graphics::MaterialInstance *, const maths::vec2 &, const maths::vec2 &, sp::graphics::MaterialInstance *, float, sp::graphics::MaterialInstance *, float, float, const maths::vec3 &, sp::graphics::MaterialInstance *);
		SP_CLASSREGISTER(Mesh, API::VertexArray *, API::IndexBuffer *, sp::graphics::MaterialInstance *);
		SP_CLASSREGISTER(Model, const String &, sp::graphics::MaterialInstance *);
		SP_FUNCTION(PBRMaterial, SetEnviromentMap, void, API::TextureCube *);
		SP_FUNCTION(PBRMaterial, SetAlbedo, void, const maths::vec4 &);
		SP_FUNCTION(PBRMaterial, SetSpecular, void, const maths::vec3 &);
		SP_FUNCTION(PBRMaterial, SetGloss, void, float);
		SP_FUNCTION(PBRMaterial, UsingNormalMap, void, bool);
		SP_FUNCTION(PBRMaterial, SetAlbedoMap, void, API::Texture2D *);
		SP_FUNCTION(PBRMaterial, SetSpecularMap, void, API::Texture2D *);
		SP_FUNCTION(PBRMaterial, SetNormalMap, void, API::Texture2D *);
		SP_FUNCTION(PBRMaterial, SetGlossMap, void, API::Texture2D *);
		SP_FUNCTION(PBRMaterialInstance, SetEnviromentMap, void, API::TextureCube *);
		SP_FUNCTION(PBRMaterialInstance, SetAlbedo, void, const maths::vec4 &);
		SP_FUNCTION(PBRMaterialInstance, SetSpecular, void, const maths::vec3 &);
		SP_FUNCTION(PBRMaterialInstance, SetGloss, void, float);
		SP_FUNCTION(PBRMaterialInstance, UsingNormalMap, void, bool);
		SP_FUNCTION(PBRMaterialInstance, SetAlbedoMap, void, API::Texture2D *);
		SP_FUNCTION(PBRMaterialInstance, SetSpecularMap, void, API::Texture2D *);
		SP_FUNCTION(PBRMaterialInstance, SetNormalMap, void, API::Texture2D *);
		SP_FUNCTION(PBRMaterialInstance, SetGlossMap, void, API::Texture2D *);
		_SP_CLASSREGISTER(VertexData);
		SP_CLASSREGISTER(Renderable2D, const maths::vec3 &, const maths::vec2 &, uint);
		_SP_CLASSREGISTER(RendererUniform);
		_SP_CLASSREGISTER(RenderCommand);
		_SP_CLASSREGISTER(Renderer);
		_SP_CLASSREGISTER(Renderable2D);
		SP_FUNCTION(Renderer2D, Push, void, const maths::mat4 &, bool);
		SP_FUNCTION(Renderer2D, Pop, void);
		SP_FUNCTION(Renderer2D, SetRenderTarget, void, sp::graphics::RenderTarget);
		SP_FUNCTION(Renderer2D, GetRenderTarget, const sp::graphics::RenderTarget);
		SP_FUNCTION(Renderer2D, SetPostEffects, void, bool);
		SP_FUNCTION(Renderer2D, GetPostEffects, bool);
		SP_FUNCTION(Renderer2D, AddPostEffectsPass, void, sp::graphics::PostEffectsPass *);
		SP_FUNCTION(Renderer2D, SetMask, void, const sp::graphics::Mask *);
		SP_FUNCTION(Renderer2D, SetCamera, void, sp::graphics::Camera *);
		SP_FUNCTION(Renderer2D, Begin, void);
		SP_FUNCTION(Renderer2D, Submit, void, const sp::graphics::Renderable2D *);
		SP_FUNCTION(Renderer2D, DrawString, void, const String &, const maths::vec2 &, const sp::graphics::Font &, uint);
		SP_FUNCTION(Renderer2D, End, void);
		SP_FUNCTION(Renderer2D, Present, void);
		_SP_CLASSREGISTER(Renderer2D);
		SP_FUNCTION(Renderer3D, Init, void);
		SP_FUNCTION(Renderer3D, Begin, void);
		SP_FUNCTION(Renderer3D, BeginScene, void, sp::graphics::Camera *);
		SP_FUNCTION(Renderer3D, Submit, void, const sp::graphics::RenderCommand &);
		SP_FUNCTION(Renderer3D, SubmitMesh, void, sp::graphics::Mesh *, const maths::mat4 &);
		SP_FUNCTION(Renderer3D, SubmitLightSetup, void, const sp::graphics::LightSetup &);
		SP_FUNCTION(Renderer3D, EndScene, void);
		SP_FUNCTION(Renderer3D, End, void);
		SP_FUNCTION(Renderer3D, Present, void);
		SP_FUNCTION(Renderer3D, SetScreenBufferSize, void, uint, uint);
		_SP_CLASSREGISTER(Renderer3D);
		_SP_CLASSREGISTER(Renderer3D);
		SP_FUNCTION(Scene, Add, void, entity::Entity *);
		SP_FUNCTION(Scene, PushLightSetup, void, sp::graphics::LightSetup *);
		SP_FUNCTION(Scene, SetCamera, void, sp::graphics::Camera *);
		SP_FUNCTION(Scene, Render, void, sp::graphics::Renderer3D &);
		SP_CLASSREGISTER(Sprite, API::Texture2D *);
		_SP_CLASSREGISTER(TextureManager);
		SP_CLASSREGISTER(Button, const String &, const maths::Rectangle &, const int &);
		_SP_CLASSREGISTER(Widget);
		SP_FUNCTION(Panel, Add, sp::graphics::ui::Widget *, sp::graphics::ui::Widget *);
		SP_FUNCTION(Panel, Remove, void, sp::graphics::ui::Widget *);
		SP_FUNCTION(Panel, Clear, void);
		SP_FUNCTION(Panel, OnEvent, void, events::Event &);
		SP_FUNCTION(Panel, OnMousePressedEvent, bool, events::MousePressedEvent &);
		SP_FUNCTION(Panel, OnMouseReleasedEvent, bool, events::MouseReleasedEvent &);
		SP_FUNCTION(Panel, OnMouseMovedEvent, bool, events::MouseMovedEvent &);
		SP_FUNCTION(Panel, OnUpdate, void);
		SP_FUNCTION(Panel, OnRender, void, sp::graphics::Renderer2D &);
		SP_CLASSREGISTER(Slider, const maths::Rectangle &, bool);
		SP_FUNCTION(Widget, OnMousePressed, bool, events::MousePressedEvent &);
		SP_FUNCTION(Widget, OnMouseReleased, bool, events::MouseReleasedEvent &);
		SP_FUNCTION(Widget, OnMouseMoved, bool, events::MouseMovedEvent &);
		SP_FUNCTION(Widget, OnRender, void, sp::graphics::Renderer2D &);
		SP_FUNCTION(Widget, SetBounds, void, const maths::Rectangle &);
		SP_FUNCTION(Widget, SetActive, void, bool);
		_SP_CLASSREGISTER(Widget);
		SP_FUNCTION(Shader, Bind, void);
		SP_FUNCTION(Shader, Unbind, void);
		SP_FUNCTION(Shader, SetVSSystemUniformBuffer, void, byte *, uint, uint);
		SP_FUNCTION(Shader, SetPSSystemUniformBuffer, void, byte *, uint, uint);
		SP_FUNCTION(Shader, SetVSUserUniformBuffer, void, byte *, uint);
		SP_FUNCTION(Shader, SetPSUserUniformBuffer, void, byte *, uint);
		SP_FUNCTION(Shader, GetVSSystemUniforms, const int &);
		SP_FUNCTION(Shader, GetPSSystemUniforms, const int &);
		SP_FUNCTION(Shader, GetVSUserUniformBuffer, const sp::graphics::API::ShaderUniformBufferDeclaration *);
		SP_FUNCTION(Shader, GetPSUserUniformBuffer, const sp::graphics::API::ShaderUniformBufferDeclaration *);
		SP_FUNCTION(Shader, GetResources, const int &);
		SP_FUNCTION(Shader, GetName, const String &);
		SP_FUNCTION(Shader, GetFilePath, const String &);
		_SP_CLASSREGISTER(Shader);
		_SP_CLASSREGISTER(ShaderManager);
		SP_FUNCTION(ShaderResourceDeclaration, GetName, const int &);
		SP_FUNCTION(ShaderResourceDeclaration, GetRegister, uint);
		SP_FUNCTION(ShaderResourceDeclaration, GetCount, uint);
		_SP_CLASSREGISTER(ShaderResourceDeclaration);
		SP_FUNCTION(ShaderUniformDeclaration, GetName, const String &);
		SP_FUNCTION(ShaderUniformDeclaration, GetSize, uint);
		SP_FUNCTION(ShaderUniformDeclaration, GetCount, uint);
		SP_FUNCTION(ShaderUniformDeclaration, GetOffset, uint);
		_SP_CLASSREGISTER(ShaderUniformDeclaration);
		SP_FUNCTION(ShaderUniformBufferDeclaration, GetName, const String &);
		SP_FUNCTION(ShaderUniformBufferDeclaration, GetRegister, uint);
		SP_FUNCTION(ShaderUniformBufferDeclaration, GetShaderType, uint);
		SP_FUNCTION(ShaderUniformBufferDeclaration, GetSize, uint);
		SP_FUNCTION(ShaderUniformBufferDeclaration, GetUniformDeclarations, const int &);
		SP_FUNCTION(ShaderUniformBufferDeclaration, FindUniform, sp::graphics::API::ShaderUniformDeclaration *, const String &);
		_SP_CLASSREGISTER(ShaderUniformBufferDeclaration);
		SP_FUNCTION(ShaderStruct, AddField, void, sp::graphics::API::ShaderUniformDeclaration *);
		SP_FUNCTION(ShaderStruct, SetOffset, void, uint);
		SP_CLASSREGISTER(ShaderStruct, const String &);
		SP_FUNCTION(PostEffects, Push, void, sp::graphics::PostEffectsPass *);
		SP_FUNCTION(PostEffects, Pop, void);
		SP_FUNCTION(PostEffects, RenderPostEffects, void, sp::graphics::Framebuffer *, sp::graphics::Framebuffer *, API::VertexArray *, API::IndexBuffer *);
		SP_FUNCTION(PostEffectsPass, RenderPass, void, sp::graphics::Framebuffer *);
		SP_FUNCTION(Group, Add, void, sp::graphics::Renderable2D *);
		SP_FUNCTION(Group, Submit, void, sp::graphics::Renderer2D *);
		SP_CLASSREGISTER(Group, const maths::mat4 &);
		SP_FUNCTION(Layer, IsVisible, bool);
		SP_FUNCTION(Layer, SetVisible, void, bool);
		SP_FUNCTION(Layer, Init, void);
		SP_FUNCTION(Layer, OnEvent, void, events::Event &);
		SP_FUNCTION(Layer, OnTick, void);
		SP_FUNCTION(Layer, OnUpdate, void);
		SP_FUNCTION(Layer, OnRender, void);
		SP_FUNCTION(Layer2D, OnInit, void, sp::graphics::Renderer2D &, sp::graphics::Material &);
		SP_FUNCTION(Layer2D, SetMask, void, const sp::graphics::Mask *);
		SP_FUNCTION(Layer2D, Add, sp::graphics::Renderable2D *, sp::graphics::Renderable2D *);
		SP_FUNCTION(Layer3D, OnInit, void, sp::graphics::Renderer3D &, sp::graphics::Scene &);
		SP_FUNCTION(Camera, SetPosition, void, const maths::vec3 &);
		SP_FUNCTION(Camera, SetRotation, void, const maths::vec3 &);
		SP_FUNCTION(Camera, SetProjectionMatrix, void, const maths::mat4 &);
		SP_CLASSREGISTER(Camera, const maths::mat4 &);
		SP_FUNCTION(MayaCamera, SetDistance, void, float);
		_SP_CLASSREGISTER(BufferElement);
		SP_CLASSREGISTER(BufferLayout, const String &, uint, bool);
		_SP_CLASSREGISTER(Context);
		SP_FUNCTION(Framebuffer, Bind, void);
		SP_FUNCTION(Framebuffer, Unbind, void);
		SP_FUNCTION(Framebuffer, Clear, void);
		SP_FUNCTION(Framebuffer, GetWidth, uint);
		SP_FUNCTION(Framebuffer, GetHeight, uint);
		SP_FUNCTION(Framebuffer, GetTexture, API::Texture *);
		_SP_CLASSREGISTER(Framebuffer);
		SP_FUNCTION(Framebuffer2D, SetClearColor, void, const maths::vec4 &);
		_SP_CLASSREGISTER(Framebuffer2D);
		_SP_CLASSREGISTER(FramebufferDepth);
		SP_FUNCTION(IndexBuffer, Bind, void);
		SP_FUNCTION(IndexBuffer, Unbind, void);
		SP_FUNCTION(IndexBuffer, GetCount, uint);
		_SP_CLASSREGISTER(IndexBuffer);
		SP_FUNCTION(Texture, Bind, void, uint);
		SP_FUNCTION(Texture, Unbind, void, uint);
		SP_FUNCTION(Texture, GetName, const String &);
		SP_FUNCTION(Texture, GetFilepath, const String &);
		_SP_CLASSREGISTER(Texture);
		SP_FUNCTION(Texture2D, GetWidth, uint);
		SP_FUNCTION(Texture2D, GetHeight, uint);
		_SP_CLASSREGISTER(Texture2D);
		_SP_CLASSREGISTER(TextureCube);
		_SP_CLASSREGISTER(TextureDepth);
		SP_FUNCTION(VertexArray, GetBuffer, API::VertexBuffer *, uint);
		SP_FUNCTION(VertexArray, PushBuffer, void, API::VertexBuffer *);
		SP_FUNCTION(VertexArray, Bind, void);
		SP_FUNCTION(VertexArray, Unbind, void);
		SP_FUNCTION(VertexArray, Draw, void, uint);
		_SP_CLASSREGISTER(VertexArray);
		SP_FUNCTION(VertexBuffer, Resize, void, uint);
		SP_FUNCTION(VertexBuffer, SetLayout, void, const sp::graphics::API::BufferLayout &);
		SP_FUNCTION(VertexBuffer, SetData, void, uint, const void *);
		SP_FUNCTION(VertexBuffer, ReleasePointer, void);
		SP_FUNCTION(VertexBuffer, Bind, void);
		SP_FUNCTION(VertexBuffer, Unbind, void);
		_SP_CLASSREGISTER(VertexBuffer);
		_SP_CLASSREGISTER(Event);
		SP_FUNCTION(IEventListener, OnEvent, void, sp::events::Event &);
		_SP_CLASSREGISTER(IEventListener);
		SP_CLASSREGISTER(KeyEvent, int32, Event::Type);
		SP_CLASSREGISTER(KeyPressedEvent, int32, int32, int32);
		SP_CLASSREGISTER(KeyReleasedEvent, int32);
		_SP_CLASSREGISTER(MouseButtonEvent);
		SP_CLASSREGISTER(MousePressedEvent, int32, float, float);
		SP_CLASSREGISTER(MouseReleasedEvent, int32, float, float);
		SP_CLASSREGISTER(MouseMovedEvent, float, float, bool);
		SP_CLASSREGISTER(ResizeWindowEvent, uint, uint);
		_SP_CLASSREGISTER(Entity);
		_SP_CLASSREGISTER(ComponentType);
		SP_FUNCTION(Component, GetEntity, sp::entity::Entity *);
		SP_FUNCTION(Component, GetType, sp::entity::component::ComponentType *);
		_SP_CLASSREGISTER(Component);
		SP_CLASSREGISTER(TransformComponent, const maths::mat4 &);
		SP_FUNCTION(DebugLayer, OnInit, void, graphics::Renderer2D &, graphics::Material &);
		SP_FUNCTION(DebugLayer, OnTick, void);
		SP_FUNCTION(DebugLayer, OnUpdate, void);
		SP_FUNCTION(DebugLayer, OnEvent, void, events::Event &);
		SP_FUNCTION(DebugLayer, OnMouseMovedEvent, bool, events::MouseMovedEvent &);
		SP_FUNCTION(DebugLayer, OnMousePressedEvent, bool, events::MousePressedEvent &);
		SP_FUNCTION(DebugLayer, OnMouseReleasedEvent, bool, events::MouseReleasedEvent &);
		SP_FUNCTION(DebugLayer, OnKeyPressedEvent, bool, events::KeyPressedEvent &);
		SP_FUNCTION(DebugLayer, OnRender, void, graphics::Renderer2D &);
		_SP_CLASSREGISTER(IAction);
		_SP_CLASSREGISTER(DebugMenuSettings);
		SP_FUNCTION(DebugMenu, OnActivate, void);
		SP_FUNCTION(DebugMenu, OnDeactivate, void);
		SP_FUNCTION(DebugMenu, EditValues, void, const String &, float *, uint, const int *);
		SP_FUNCTION(DebugMenu, OnMousePressed, bool, events::MousePressedEvent &);
		SP_FUNCTION(DebugMenu, OnMouseReleased, bool, events::MouseReleasedEvent &);
		SP_FUNCTION(DebugMenu, OnUpdate, void);
		SP_FUNCTION(DebugMenu, OnRender, void, graphics::Renderer2D &);
		_SP_CLASSREGISTER(DebugMenu);
		SP_FUNCTION(IAction, OnAction, void);
		SP_FUNCTION(IAction, ToString, String);
		_SP_CLASSREGISTER(IAction);
		SP_CLASSREGISTER(EmptyAction, const String &);
		SP_CLASSREGISTER(BooleanAction, const String &, const int &, const int &);
		_SP_CLASSREGISTER(IAction);
		SP_CLASSREGISTER(DebugMenuItem, sp::debug::IAction *, const maths::Rectangle &);
		_SP_CLASSREGISTER(DebugMenuSlider);
		_SP_CLASSREGISTER(DebugRenderer);
		_SP_CLASSREGISTER(ga_Sound);
		_SP_CLASSREGISTER(ga_Handle);
		SP_CLASSREGISTER(Sound, const String &, const String &);
		_SP_CLASSREGISTER(gau_Manager);
		_SP_CLASSREGISTER(ga_Mixer);
		_SP_CLASSREGISTER(SoundManager);
		_SP_CLASSREGISTER(DebugLayer);
		SP_CLASSREGISTER(Application, const String &, const sp::WindowProperties &, graphics::API::RenderAPI);
		_SP_CLASSREGISTER(Window);
		SP_FUNCTION(InputManager, SetEventCallback, void, const int &);
		SP_FUNCTION(InputManager, Update, void);
		SP_FUNCTION(InputManager, PlatformUpdate, void);
		SP_FUNCTION(InputManager, IsKeyPressed, bool, uint);
		SP_FUNCTION(InputManager, IsMouseButtonPressed, bool, uint);
		SP_FUNCTION(InputManager, IsMouseButtonClicked, bool, uint);
		SP_FUNCTION(InputManager, GetMousePosition, const maths::vec2 &);
		SP_FUNCTION(InputManager, SetMousePosition, void, const maths::vec2 &);
		SP_FUNCTION(InputManager, IsMouseGrabbed, const bool);
		SP_FUNCTION(InputManager, SetMouseGrabbed, void, bool);
		SP_FUNCTION(InputManager, SetMouseCursor, void, int32);
		SP_FUNCTION(InputManager, ClearKeys, void);
		SP_FUNCTION(InputManager, ClearMouseButtons, void);
		_SP_CLASSREGISTER(Input);
		_SP_CLASSREGISTER(WindowProperties);
		SP_CLASSREGISTER(Window, const String &, const sp::WindowProperties &);
	}
} }
