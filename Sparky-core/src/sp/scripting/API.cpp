#include "sp\sp.h"
#include "API.h"
#include <lua.hpp>
#include <luabind\luabind.hpp>

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
			luabind::class_<AABB>("AABB")
				.def(luabind::constructor<>())
				.def("Intersects", (bool(AABB::*)(const sp::maths::AABB &))&AABB::Intersects)
				.def("Center", (sp::maths::vec3(AABB::*)())&AABB::Center),

			luabind::class_<Allocator>("Allocator"),

			luabind::class_<Application>("Application")
				.def(luabind::constructor<const String &, const sp::WindowProperties &, graphics::API::RenderAPI>())
				.def("Init", (void(Application::*)())&Application::Init)
				.def("PushLayer", (void(Application::*)(graphics::Layer *))&Application::PushLayer)
				.def("PopLayer", (graphics::Layer *(Application::*)())&Application::PopLayer)
				.def("PopLayer", (graphics::Layer *(Application::*)(graphics::Layer *))&Application::PopLayer)
				.def("PushOverlay", (void(Application::*)(graphics::Layer *))&Application::PushOverlay)
				.def("PopOverlay", (graphics::Layer *(Application::*)())&Application::PopOverlay)
				.def("PopOverlay", (graphics::Layer *(Application::*)(graphics::Layer *))&Application::PopOverlay)
				.def("Start", (void(Application::*)())&Application::Start)
				.def("Suspend", (void(Application::*)())&Application::Suspend)
				.def("Resume", (void(Application::*)())&Application::Resume)
				.def("Stop", (void(Application::*)())&Application::Stop)
				.def("GetFPS", (uint(Application::*)())&Application::GetFPS)
				.def("GetUPS", (uint(Application::*)())&Application::GetUPS)
				.def("GetFrametime", (float(Application::*)())&Application::GetFrametime)
				.def("GetWindowWidth", (uint(Application::*)())&Application::GetWindowWidth)
				.def("GetWindowHeight", (uint(Application::*)())&Application::GetWindowHeight)
				.def("GetWindowSize", (maths::vec2(Application::*)())&Application::GetWindowSize)
				.def("GetBuildConfiguration", (String(Application::*)())&Application::GetBuildConfiguration)
				.def("GetPlatform", (String(Application::*)())&Application::GetPlatform),

			luabind::class_<BR2DSystemUniform>("BR2DSystemUniform")
				.def(luabind::constructor<>()),

			luabind::class_<Renderer2D>("Renderer2D")
				.def("Push", (void(Renderer2D::*)(const maths::mat4 &, bool))&Renderer2D::Push)
				.def("Pop", (void(Renderer2D::*)())&Renderer2D::Pop)
				.def("SetPostEffects", (void(Renderer2D::*)(bool))&Renderer2D::SetPostEffects)
				.def("GetPostEffects", (bool(Renderer2D::*)())&Renderer2D::GetPostEffects)
				.def("AddPostEffectsPass", (void(Renderer2D::*)(sp::graphics::PostEffectsPass *))&Renderer2D::AddPostEffectsPass)
				.def("SetMask", (void(Renderer2D::*)(const sp::graphics::Mask *))&Renderer2D::SetMask)
				.def("SetCamera", (void(Renderer2D::*)(sp::graphics::Camera *))&Renderer2D::SetCamera)
				.def("Begin", (void(Renderer2D::*)())&Renderer2D::Begin)
				.def("Submit", (void(Renderer2D::*)(const sp::graphics::Renderable2D *))&Renderer2D::Submit)
				.def("DrawLine", (void(Renderer2D::*)(float, float, float, float, uint, float))&Renderer2D::DrawLine)
				.def("DrawLine", (void(Renderer2D::*)(const maths::vec2 &, const maths::vec2 &, uint, float))&Renderer2D::DrawLine)
				.def("DrawRect", (void(Renderer2D::*)(float, float, float, float, uint))&Renderer2D::DrawRect)
				.def("DrawRect", (void(Renderer2D::*)(const maths::Rectangle &, uint))&Renderer2D::DrawRect)
				.def("DrawString", (void(Renderer2D::*)(const String &, const maths::vec2 &, const sp::graphics::Font &, uint))&Renderer2D::DrawString)
				.def("FillRect", (void(Renderer2D::*)(float, float, float, float, uint))&Renderer2D::FillRect)
				.def("FillRect", (void(Renderer2D::*)(const maths::Rectangle &, uint))&Renderer2D::FillRect)
				.def("End", (void(Renderer2D::*)())&Renderer2D::End)
				.def("Present", (void(Renderer2D::*)())&Renderer2D::Present),

			luabind::class_<BatchRenderer2D, Renderer2D>("BatchRenderer2D")
				.def(luabind::constructor<uint, uint>())
				.def("SetCamera", (void(BatchRenderer2D::*)(sp::graphics::Camera *))&BatchRenderer2D::SetCamera)
				.def("Begin", (void(BatchRenderer2D::*)())&BatchRenderer2D::Begin)
				.def("Submit", (void(BatchRenderer2D::*)(const sp::graphics::Renderable2D *))&BatchRenderer2D::Submit)
				.def("DrawLine", (void(BatchRenderer2D::*)(float, float, float, float, uint, float))&BatchRenderer2D::DrawLine)
				.def("DrawLine", (void(BatchRenderer2D::*)(const maths::vec2 &, const maths::vec2 &, uint, float))&BatchRenderer2D::DrawLine)
				.def("DrawRect", (void(BatchRenderer2D::*)(float, float, float, float, uint))&BatchRenderer2D::DrawRect)
				.def("DrawRect", (void(BatchRenderer2D::*)(const maths::Rectangle &, uint))&BatchRenderer2D::DrawRect)
				.def("DrawString", (void(BatchRenderer2D::*)(const String &, const maths::vec2 &, const sp::graphics::Font &, uint))&BatchRenderer2D::DrawString)
				.def("FillRect", (void(BatchRenderer2D::*)(float, float, float, float, uint))&BatchRenderer2D::FillRect)
				.def("FillRect", (void(BatchRenderer2D::*)(const maths::Rectangle &, uint))&BatchRenderer2D::FillRect)
				.def("End", (void(BatchRenderer2D::*)())&BatchRenderer2D::End)
				.def("Present", (void(BatchRenderer2D::*)())&BatchRenderer2D::Present)
				.def("SetScreenSize", (void(BatchRenderer2D::*)(const maths::tvec2<uint> &))&BatchRenderer2D::SetScreenSize)
				.def("GetScreenSize", (const maths::tvec2<uint> &(BatchRenderer2D::*)())&BatchRenderer2D::GetScreenSize)
				.def("SetViewportSize", (void(BatchRenderer2D::*)(const maths::tvec2<uint> &))&BatchRenderer2D::SetViewportSize)
				.def("GetViewportSize", (const maths::tvec2<uint> &(BatchRenderer2D::*)())&BatchRenderer2D::GetViewportSize),

			luabind::class_<Camera>("Camera")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("Focus", (void(Camera::*)())&Camera::Focus)
				.def("Update", (void(Camera::*)())&Camera::Update)
				.def("GetPosition", (const maths::vec3 &(Camera::*)())&Camera::GetPosition)
				.def("SetPosition", (void(Camera::*)(const maths::vec3 &))&Camera::SetPosition)
				.def("GetRotation", (const maths::vec3 &(Camera::*)())&Camera::GetRotation)
				.def("SetRotation", (void(Camera::*)(const maths::vec3 &))&Camera::SetRotation)
				.def("GetProjectionMatrix", (const maths::mat4 &(Camera::*)())&Camera::GetProjectionMatrix)
				.def("SetProjectionMatrix", (void(Camera::*)(const maths::mat4 &))&Camera::SetProjectionMatrix)
				.def("Translate", (void(Camera::*)(const maths::vec3 &))&Camera::Translate)
				.def("Rotate", (void(Camera::*)(const maths::vec3 &))&Camera::Rotate)
				.def("Translate", (void(Camera::*)(float, float, float))&Camera::Translate)
				.def("Rotate", (void(Camera::*)(float, float, float))&Camera::Rotate)
				.def("GetFocalPoint", (const maths::vec3 &(Camera::*)())&Camera::GetFocalPoint)
				.def("GetViewMatrix", (const maths::mat4 &(Camera::*)())&Camera::GetViewMatrix),

			luabind::class_<Component>("Component")
				.def("GetEntity", (sp::entity::Entity *(Component::*)())&Component::GetEntity)
				.def("GetType", (sp::entity::component::ComponentType *(Component::*)())&Component::GetType),

			luabind::class_<Context>("Context"),

			luabind::class_<DebugMenu>("DebugMenu")
				.def("OnActivate", (void(DebugMenu::*)())&DebugMenu::OnActivate)
				.def("OnDeactivate", (void(DebugMenu::*)())&DebugMenu::OnDeactivate)
				.def("EditValues", (void(DebugMenu::*)(const String &, float *, uint, const int *))&DebugMenu::EditValues)
				.def("OnMousePressed", (bool(DebugMenu::*)(events::MousePressedEvent &))&DebugMenu::OnMousePressed)
				.def("OnMouseReleased", (bool(DebugMenu::*)(events::MouseReleasedEvent &))&DebugMenu::OnMouseReleased)
				.def("OnUpdate", (void(DebugMenu::*)())&DebugMenu::OnUpdate)
				.def("OnRender", (void(DebugMenu::*)(graphics::Renderer2D &))&DebugMenu::OnRender),

			luabind::class_<DebugRenderer>("DebugRenderer"),

			luabind::class_<FPSCamera, Camera>("FPSCamera")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("Focus", (void(FPSCamera::*)())&FPSCamera::Focus)
				.def("Update", (void(FPSCamera::*)())&FPSCamera::Update),

			luabind::class_<FileSystem>("FileSystem"),

			luabind::class_<Font>("Font")
				.def(luabind::constructor<const String &, const String &, float>())
				.def("SetScale", (void(Font::*)(float, float))&Font::SetScale)
				.def("SetScale", (void(Font::*)(const maths::vec2 &))&Font::SetScale)
				.def("GetScale", (const maths::vec2 &(Font::*)())&Font::GetScale)
				.def("GetName", (const String &(Font::*)())&Font::GetName)
				.def("GetFileName", (const String &(Font::*)())&Font::GetFileName)
				.def("GetSize", (float(Font::*)())&Font::GetSize)
				.def("GetOffsets", (maths::vec2(Font::*)(const String &))&Font::GetOffsets)
				.def("GetWidth", (float(Font::*)(const String &))&Font::GetWidth)
				.def("GetHeight", (float(Font::*)(const String &))&Font::GetHeight)
				.def("GetBounds", (maths::Rectangle(Font::*)(const String &))&Font::GetBounds)
				.def("GetTexture", (API::Texture2D *(Font::*)())&Font::GetTexture),

			luabind::class_<FontManager>("FontManager"),

			luabind::class_<Framebuffer>("Framebuffer")
				.def("Bind", (void(Framebuffer::*)())&Framebuffer::Bind)
				.def("Unbind", (void(Framebuffer::*)())&Framebuffer::Unbind)
				.def("Clear", (void(Framebuffer::*)())&Framebuffer::Clear)
				.def("GetWidth", (uint(Framebuffer::*)())&Framebuffer::GetWidth)
				.def("GetHeight", (uint(Framebuffer::*)())&Framebuffer::GetHeight)
				.def("GetTexture", (API::Texture *(Framebuffer::*)())&Framebuffer::GetTexture),

			luabind::class_<Framebuffer2D, Framebuffer>("Framebuffer2D")
				.def("SetClearColor", (void(Framebuffer2D::*)(const maths::vec4 &))&Framebuffer2D::SetClearColor),

			luabind::class_<FramebufferDepth, Framebuffer>("FramebufferDepth"),

			luabind::class_<Renderable2D>("Renderable2D"),

			luabind::class_<Group, Renderable2D>("Group")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("Add", (void(Group::*)(sp::graphics::Renderable2D *))&Group::Add)
				.def("Submit", (void(Group::*)(sp::graphics::Renderer2D *))&Group::Submit)
				.def("GetTransformRef", (maths::mat4 &(Group::*)())&Group::GetTransformRef),

			luabind::class_<IRenderable>("IRenderable")
				.def("Render", (void(IRenderable::*)(sp::graphics::Renderer3D &))&IRenderable::Render),

			luabind::class_<IndexBuffer>("IndexBuffer")
				.def("Bind", (void(IndexBuffer::*)())&IndexBuffer::Bind)
				.def("Unbind", (void(IndexBuffer::*)())&IndexBuffer::Unbind)
				.def("GetCount", (uint(IndexBuffer::*)())&IndexBuffer::GetCount),

			luabind::class_<Input>("Input"),

			luabind::class_<InputManager>("InputManager")
				.def(luabind::constructor<>())
				.def("SetEventCallback", (void(InputManager::*)(const int &))&InputManager::SetEventCallback)
				.def("Update", (void(InputManager::*)())&InputManager::Update)
				.def("PlatformUpdate", (void(InputManager::*)())&InputManager::PlatformUpdate)
				.def("IsKeyPressed", (bool(InputManager::*)(uint))&InputManager::IsKeyPressed)
				.def("IsMouseButtonPressed", (bool(InputManager::*)(uint))&InputManager::IsMouseButtonPressed)
				.def("IsMouseButtonClicked", (bool(InputManager::*)(uint))&InputManager::IsMouseButtonClicked)
				.def("GetMousePosition", (const maths::vec2 &(InputManager::*)())&InputManager::GetMousePosition)
				.def("SetMousePosition", (void(InputManager::*)(const maths::vec2 &))&InputManager::SetMousePosition)
				.def("IsMouseGrabbed", (const bool(InputManager::*)())&InputManager::IsMouseGrabbed)
				.def("SetMouseGrabbed", (void(InputManager::*)(bool))&InputManager::SetMouseGrabbed)
				.def("SetMouseCursor", (void(InputManager::*)(int32))&InputManager::SetMouseCursor)
				.def("ClearKeys", (void(InputManager::*)())&InputManager::ClearKeys)
				.def("ClearMouseButtons", (void(InputManager::*)())&InputManager::ClearMouseButtons),

			luabind::class_<Label, Renderable2D>("Label")
				.def(luabind::constructor<const String &, float, float, uint>())
				.def("Submit", (void(Label::*)(sp::graphics::Renderer2D *))&Label::Submit)
				.def("ValidateFont", (void(Label::*)(const String &, int32))&Label::ValidateFont)
				.def("GetFont", (const sp::graphics::Font &(Label::*)())&Label::GetFont),

			luabind::class_<Light>("Light")
				.def(luabind::constructor<const maths::vec3 &, float, const maths::vec4 &>()),

			luabind::class_<LightSetup>("LightSetup")
				.def(luabind::constructor<>())
				.def("Add", (sp::graphics::Light *(LightSetup::*)(sp::graphics::Light *))&LightSetup::Add)
				.def("Remove", (void(LightSetup::*)(sp::graphics::Light *))&LightSetup::Remove),

			luabind::class_<Mask>("Mask")
				.def(luabind::constructor<API::Texture2D *, const maths::mat4 &>()),

			luabind::class_<Material>("Material")
				.def(luabind::constructor<API::Shader *>())
				.def("Bind", (void(Material::*)())&Material::Bind)
				.def("Unbind", (void(Material::*)())&Material::Unbind)
				.def("SetUniformData", (void(Material::*)(const String &, byte *))&Material::SetUniformData)
				.def("SetTexture", (void(Material::*)(const String &, API::Texture *))&Material::SetTexture)
				.def("GetRenderFlags", (int(Material::*)())&Material::GetRenderFlags)
				.def("SetRenderFlags", (void(Material::*)(int))&Material::SetRenderFlags)
				.def("SetRenderFlag", (void(Material::*)(Material::RenderFlags))&Material::SetRenderFlag),

			luabind::class_<MayaCamera, Camera>("MayaCamera")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("Focus", (void(MayaCamera::*)())&MayaCamera::Focus)
				.def("Update", (void(MayaCamera::*)())&MayaCamera::Update)
				.def("GetDistance", (float(MayaCamera::*)())&MayaCamera::GetDistance)
				.def("SetDistance", (void(MayaCamera::*)(float))&MayaCamera::SetDistance),

			luabind::class_<Members>("Members"),

			luabind::class_<MemoryManager>("MemoryManager")
				.def(luabind::constructor<>())
				.def("GetMemoryStats", (sp::internal::MemoryStats(MemoryManager::*)())&MemoryManager::GetMemoryStats)
				.def("GetSystemInfo", (sp::internal::SystemMemoryInfo(MemoryManager::*)())&MemoryManager::GetSystemInfo),

			luabind::class_<MemoryStats>("MemoryStats")
				.def(luabind::constructor<>()),

			luabind::class_<Mesh, IRenderable>("Mesh")
				.def(luabind::constructor<API::VertexArray *, API::IndexBuffer *, sp::graphics::MaterialInstance *>())
				.def("SetMaterial", (void(Mesh::*)(sp::graphics::MaterialInstance *))&Mesh::SetMaterial)
				.def("GetMaterialInstance", (sp::graphics::MaterialInstance *(Mesh::*)())&Mesh::GetMaterialInstance)
				.def("Render", (void(Mesh::*)(sp::graphics::Renderer3D &, float, float, float, float, sp::graphics::MaterialInstance *, const maths::vec2 &, const maths::vec2 &, sp::graphics::MaterialInstance *, float, sp::graphics::MaterialInstance *, float, float, const maths::vec3 &, sp::graphics::MaterialInstance *))&Mesh::Render),

			luabind::class_<MeshComponent, Component>("MeshComponent")
				.def(luabind::constructor<graphics::Mesh *>())
				.def("GetType", (sp::entity::component::ComponentType *(MeshComponent::*)())&MeshComponent::GetType),

			luabind::class_<Model, IRenderable>("Model")
				.def(luabind::constructor<const String &, sp::graphics::MaterialInstance *>())
				.def("Render", (void(Model::*)(sp::graphics::Renderer3D &))&Model::Render)
				.def("GetMesh", (sp::graphics::Mesh *(Model::*)())&Model::GetMesh),

			luabind::class_<PBRMaterial, Material>("PBRMaterial")
				.def(luabind::constructor<API::Shader *>())
				.def("SetEnviromentMap", (void(PBRMaterial::*)(API::TextureCube *))&PBRMaterial::SetEnviromentMap)
				.def("SetAlbedo", (void(PBRMaterial::*)(const maths::vec4 &))&PBRMaterial::SetAlbedo)
				.def("SetSpecular", (void(PBRMaterial::*)(const maths::vec3 &))&PBRMaterial::SetSpecular)
				.def("SetGloss", (void(PBRMaterial::*)(float))&PBRMaterial::SetGloss)
				.def("UsingNormalMap", (void(PBRMaterial::*)(bool))&PBRMaterial::UsingNormalMap)
				.def("SetAlbedoMap", (void(PBRMaterial::*)(API::Texture2D *))&PBRMaterial::SetAlbedoMap)
				.def("SetSpecularMap", (void(PBRMaterial::*)(API::Texture2D *))&PBRMaterial::SetSpecularMap)
				.def("SetNormalMap", (void(PBRMaterial::*)(API::Texture2D *))&PBRMaterial::SetNormalMap)
				.def("SetGlossMap", (void(PBRMaterial::*)(API::Texture2D *))&PBRMaterial::SetGlossMap)
				.def("GetAlbedoMap", (API::Texture *(PBRMaterial::*)())&PBRMaterial::GetAlbedoMap)
				.def("GetSpecularMap", (API::Texture *(PBRMaterial::*)())&PBRMaterial::GetSpecularMap)
				.def("GetNormalMap", (API::Texture *(PBRMaterial::*)())&PBRMaterial::GetNormalMap)
				.def("GetGlossMap", (API::Texture *(PBRMaterial::*)())&PBRMaterial::GetGlossMap),

			luabind::class_<PostEffects>("PostEffects")
				.def(luabind::constructor<>())
				.def("Push", (void(PostEffects::*)(sp::graphics::PostEffectsPass *))&PostEffects::Push)
				.def("Pop", (void(PostEffects::*)())&PostEffects::Pop)
				.def("RenderPostEffects", (void(PostEffects::*)(sp::graphics::Framebuffer *, sp::graphics::Framebuffer *, API::VertexArray *, API::IndexBuffer *))&PostEffects::RenderPostEffects),

			luabind::class_<PostEffectsPass>("PostEffectsPass")
				.def(luabind::constructor<API::Shader *>())
				.def("RenderPass", (void(PostEffectsPass::*)(sp::graphics::Framebuffer *))&PostEffectsPass::RenderPass),

			luabind::class_<Quaternion>("Quaternion")
				.def(luabind::constructor<>())
				.def("SetXYZ", (sp::maths::Quaternion &(Quaternion::*)(const sp::maths::vec3 &))&Quaternion::SetXYZ)
				.def("GetXYZ", (const sp::maths::vec3(Quaternion::*)())&Quaternion::GetXYZ)
				.def("SetElem", (sp::maths::Quaternion &(Quaternion::*)(int32, float))&Quaternion::SetElem)
				.def("GetElem", (float(Quaternion::*)(int32))&Quaternion::GetElem)
				.def("GetAxis", (sp::maths::vec3(Quaternion::*)())&Quaternion::GetAxis)
				.def("ToEulerAngles", (sp::maths::vec3(Quaternion::*)())&Quaternion::ToEulerAngles)
				.def("Dot", (float(Quaternion::*)(const sp::maths::Quaternion &))&Quaternion::Dot)
				.def("Conjugate", (sp::maths::Quaternion(Quaternion::*)())&Quaternion::Conjugate),

			luabind::class_<Rectangle>("Rectangle"),

			luabind::class_<Renderer>("Renderer"),

			luabind::class_<Scene>("Scene")
				.def(luabind::constructor<>())
				.def("Add", (void(Scene::*)(entity::Entity *))&Scene::Add)
				.def("PushLightSetup", (void(Scene::*)(sp::graphics::LightSetup *))&Scene::PushLightSetup)
				.def("PopLightSetup", (sp::graphics::LightSetup *(Scene::*)())&Scene::PopLightSetup)
				.def("SetCamera", (void(Scene::*)(sp::graphics::Camera *))&Scene::SetCamera)
				.def("Update", (void(Scene::*)())&Scene::Update)
				.def("Render", (void(Scene::*)(sp::graphics::Renderer3D &))&Scene::Render)
				.def("GetCamera", (sp::graphics::Camera *(Scene::*)())&Scene::GetCamera),

			luabind::class_<Shader>("Shader")
				.def("Bind", (void(Shader::*)())&Shader::Bind)
				.def("Unbind", (void(Shader::*)())&Shader::Unbind)
				.def("SetVSSystemUniformBuffer", (void(Shader::*)(byte *, uint, uint))&Shader::SetVSSystemUniformBuffer)
				.def("SetPSSystemUniformBuffer", (void(Shader::*)(byte *, uint, uint))&Shader::SetPSSystemUniformBuffer)
				.def("SetVSUserUniformBuffer", (void(Shader::*)(byte *, uint))&Shader::SetVSUserUniformBuffer)
				.def("SetPSUserUniformBuffer", (void(Shader::*)(byte *, uint))&Shader::SetPSUserUniformBuffer)
				.def("GetVSSystemUniforms", (const int &(Shader::*)())&Shader::GetVSSystemUniforms)
				.def("GetPSSystemUniforms", (const int &(Shader::*)())&Shader::GetPSSystemUniforms)
				.def("GetVSUserUniformBuffer", (const sp::graphics::API::ShaderUniformBufferDeclaration *(Shader::*)())&Shader::GetVSUserUniformBuffer)
				.def("GetPSUserUniformBuffer", (const sp::graphics::API::ShaderUniformBufferDeclaration *(Shader::*)())&Shader::GetPSUserUniformBuffer)
				.def("GetResources", (const int &(Shader::*)())&Shader::GetResources)
				.def("GetName", (const String &(Shader::*)())&Shader::GetName)
				.def("GetFilePath", (const String &(Shader::*)())&Shader::GetFilePath),

			luabind::class_<ShaderManager>("ShaderManager"),

			luabind::class_<ShaderResourceDeclaration>("ShaderResourceDeclaration")
				.def("GetName", (const int &(ShaderResourceDeclaration::*)())&ShaderResourceDeclaration::GetName)
				.def("GetRegister", (uint(ShaderResourceDeclaration::*)())&ShaderResourceDeclaration::GetRegister)
				.def("GetCount", (uint(ShaderResourceDeclaration::*)())&ShaderResourceDeclaration::GetCount),

			luabind::class_<ShaderStruct>("ShaderStruct")
				.def(luabind::constructor<const String &>())
				.def("AddField", (void(ShaderStruct::*)(sp::graphics::API::ShaderUniformDeclaration *))&ShaderStruct::AddField)
				.def("SetOffset", (void(ShaderStruct::*)(uint))&ShaderStruct::SetOffset)
				.def("GetName", (const String &(ShaderStruct::*)())&ShaderStruct::GetName)
				.def("GetSize", (uint(ShaderStruct::*)())&ShaderStruct::GetSize)
				.def("GetOffset", (uint(ShaderStruct::*)())&ShaderStruct::GetOffset),

			luabind::class_<ShaderUniformBufferDeclaration>("ShaderUniformBufferDeclaration")
				.def("GetName", (const String &(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetName)
				.def("GetRegister", (uint(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetRegister)
				.def("GetShaderType", (uint(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetShaderType)
				.def("GetSize", (uint(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetSize)
				.def("GetUniformDeclarations", (const int &(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetUniformDeclarations)
				.def("FindUniform", (sp::graphics::API::ShaderUniformDeclaration *(ShaderUniformBufferDeclaration::*)(const String &))&ShaderUniformBufferDeclaration::FindUniform),

			luabind::class_<ShaderUniformDeclaration>("ShaderUniformDeclaration")
				.def("GetName", (const String &(ShaderUniformDeclaration::*)())&ShaderUniformDeclaration::GetName)
				.def("GetSize", (uint(ShaderUniformDeclaration::*)())&ShaderUniformDeclaration::GetSize)
				.def("GetCount", (uint(ShaderUniformDeclaration::*)())&ShaderUniformDeclaration::GetCount)
				.def("GetOffset", (uint(ShaderUniformDeclaration::*)())&ShaderUniformDeclaration::GetOffset),

			luabind::class_<Sound>("Sound")
				.def(luabind::constructor<const String &, const String &>())
				.def("Play", (void(Sound::*)())&Sound::Play)
				.def("Loop", (void(Sound::*)())&Sound::Loop)
				.def("Pause", (void(Sound::*)())&Sound::Pause)
				.def("Resume", (void(Sound::*)())&Sound::Resume)
				.def("Stop", (void(Sound::*)())&Sound::Stop)
				.def("SetGain", (void(Sound::*)(float))&Sound::SetGain)
				.def("IsPlaying", (const bool(Sound::*)())&Sound::IsPlaying)
				.def("GetGain", (const float(Sound::*)())&Sound::GetGain)
				.def("GetName", (const String &(Sound::*)())&Sound::GetName)
				.def("GetFileName", (const String &(Sound::*)())&Sound::GetFileName),

			luabind::class_<SoundManager>("SoundManager")
				.scope[
					luabind::def("Add", &SoundManager::Add),
					luabind::def("Get", &SoundManager::Get)
				],

			luabind::class_<Sprite, Renderable2D>("Sprite")
				.def(luabind::constructor<API::Texture2D *>())
				.def("SetUV", (void(Sprite::*)(const int))&Sprite::SetUV)
				.def("SetTexture", (void(Sprite::*)(API::Texture2D *))&Sprite::SetTexture),

			luabind::class_<StringFormat>("StringFormat"),

			luabind::class_<System>("System"),

			luabind::class_<SystemInfo>("SystemInfo"),

			luabind::class_<SystemMemoryInfo>("SystemMemoryInfo")
				.def("Log", (void(SystemMemoryInfo::*)())&SystemMemoryInfo::Log),

			luabind::class_<Texture>("Texture")
				.def("Bind", (void(Texture::*)(uint))&Texture::Bind)
				.def("Unbind", (void(Texture::*)(uint))&Texture::Unbind)
				.def("GetName", (const String &(Texture::*)())&Texture::GetName)
				.def("GetFilepath", (const String &(Texture::*)())&Texture::GetFilepath),

			luabind::class_<Texture2D, Texture>("Texture2D")
				.def("SetData", (void(Texture2D::*)(const void *))&Texture2D::SetData)
				.def("SetData", (void(Texture2D::*)(uint))&Texture2D::SetData)
				.def("GetWidth", (uint(Texture2D::*)())&Texture2D::GetWidth)
				.def("GetHeight", (uint(Texture2D::*)())&Texture2D::GetHeight),

			luabind::class_<TextureCube, Texture>("TextureCube"),

			luabind::class_<TextureDepth, Texture>("TextureDepth"),

			luabind::class_<TextureLoadOptions>("TextureLoadOptions")
				.def(luabind::constructor<>()),

			luabind::class_<TextureManager>("TextureManager"),

			luabind::class_<TextureParameters>("TextureParameters")
				.def(luabind::constructor<>()),

			luabind::class_<Timer>("Timer")
				.def(luabind::constructor<>())
				.def("Reset", (void(Timer::*)())&Timer::Reset)
				.def("Elapsed", (float(Timer::*)())&Timer::Elapsed)
				.def("ElapsedMillis", (float(Timer::*)())&Timer::ElapsedMillis),

			luabind::class_<TransformComponent, Component>("TransformComponent")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("GetType", (sp::entity::component::ComponentType *(TransformComponent::*)())&TransformComponent::GetType),

			luabind::class_<UniformBuffer>("UniformBuffer")
				.def(luabind::constructor<>()),

			luabind::class_<VertexArray>("VertexArray")
				.def("GetBuffer", (API::VertexBuffer *(VertexArray::*)(uint))&VertexArray::GetBuffer)
				.def("PushBuffer", (void(VertexArray::*)(API::VertexBuffer *))&VertexArray::PushBuffer)
				.def("Bind", (void(VertexArray::*)())&VertexArray::Bind)
				.def("Unbind", (void(VertexArray::*)())&VertexArray::Unbind)
				.def("Draw", (void(VertexArray::*)(uint))&VertexArray::Draw),

			luabind::class_<VertexBuffer>("VertexBuffer")
				.def("Resize", (void(VertexBuffer::*)(uint))&VertexBuffer::Resize)
				.def("SetLayout", (void(VertexBuffer::*)(const sp::graphics::API::BufferLayout &))&VertexBuffer::SetLayout)
				.def("SetData", (void(VertexBuffer::*)(uint, const void *))&VertexBuffer::SetData)
				.def("ReleasePointer", (void(VertexBuffer::*)())&VertexBuffer::ReleasePointer)
				.def("Bind", (void(VertexBuffer::*)())&VertexBuffer::Bind)
				.def("Unbind", (void(VertexBuffer::*)())&VertexBuffer::Unbind),

			luabind::class_<Window>("Window")
				.def(luabind::constructor<const String &, const sp::WindowProperties &>())
				.def("Clear", (void(Window::*)())&Window::Clear)
				.def("Update", (void(Window::*)())&Window::Update)
				.def("Closed", (bool(Window::*)())&Window::Closed)
				.def("SetTitle", (void(Window::*)(const String &))&Window::SetTitle)
				.def("GetWidth", (uint(Window::*)())&Window::GetWidth)
				.def("GetHeight", (uint(Window::*)())&Window::GetHeight)
				.def("SetVsync", (void(Window::*)(bool))&Window::SetVsync)
				.def("IsVsync", (bool(Window::*)())&Window::IsVsync)
				.def("GetInputManager", (sp::InputManager *(Window::*)())&Window::GetInputManager)
				.def("SetEventCallback", (void(Window::*)(const int &))&Window::SetEventCallback),

			luabind::class_<mat4>("mat4"),

			luabind::class_<vec2>("vec2"),

			luabind::class_<vec3>("vec3")
				.def(luabind::constructor<>())
				.def("Add", (sp::maths::vec3 &(vec3::*)(const sp::maths::vec3 &))&vec3::Add)
				.def("Subtract", (sp::maths::vec3 &(vec3::*)(const sp::maths::vec3 &))&vec3::Subtract)
				.def("Divide", (sp::maths::vec3 &(vec3::*)(const sp::maths::vec3 &))&vec3::Divide)
				.def("Add", (sp::maths::vec3 &(vec3::*)(float))&vec3::Add)
				.def("Subtract", (sp::maths::vec3 &(vec3::*)(float))&vec3::Subtract)
				.def("Divide", (sp::maths::vec3 &(vec3::*)(float))&vec3::Divide)
				.def("Cross", (sp::maths::vec3(vec3::*)(const sp::maths::vec3 &))&vec3::Cross)
				.def("Dot", (float(vec3::*)(const sp::maths::vec3 &))&vec3::Dot)
				.def("Magnitude", (float(vec3::*)())&vec3::Magnitude)
				.def("Normalize", (sp::maths::vec3(vec3::*)())&vec3::Normalize)
				.def("Distance", (float(vec3::*)(const sp::maths::vec3 &))&vec3::Distance),

			luabind::class_<vec4>("vec4")
				.def(luabind::constructor<>())
				.def("Add", (sp::maths::vec4 &(vec4::*)(const sp::maths::vec4 &))&vec4::Add)
				.def("Subtract", (sp::maths::vec4 &(vec4::*)(const sp::maths::vec4 &))&vec4::Subtract)
				.def("Divide", (sp::maths::vec4 &(vec4::*)(const sp::maths::vec4 &))&vec4::Divide)
				.def("Dot", (float(vec4::*)(const sp::maths::vec4 &))&vec4::Dot)
		];
	}
} }