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

			luabind::class_<Timer>("Timer")
				.def(luabind::constructor<>())
				.def("Reset", (void(Timer::*)())&Timer::Reset)
				.def("Elapsed", (float(Timer::*)())&Timer::Elapsed)
				.def("ElapsedMillis", (float(Timer::*)())&Timer::ElapsedMillis),

			luabind::class_<Allocator>("Allocator"),

			luabind::class_<FileSystem>("FileSystem"),

			luabind::class_<SystemMemoryInfo>("SystemMemoryInfo")
				.def("Log", (void(SystemMemoryInfo::*)())&SystemMemoryInfo::Log),

			luabind::class_<MemoryStats>("MemoryStats")
				.def(luabind::constructor<>()),

			luabind::class_<MemoryManager>("MemoryManager")
				.def(luabind::constructor<>())
				.def("GetMemoryStats", (sp::internal::MemoryStats(MemoryManager::*)())&MemoryManager::GetMemoryStats)
				.def("GetSystemInfo", (sp::internal::SystemMemoryInfo(MemoryManager::*)())&MemoryManager::GetSystemInfo),

			luabind::class_<SystemInfo>("SystemInfo"),

			luabind::class_<System>("System"),

			luabind::class_<AABB>("AABB")
				.def(luabind::constructor<>())
				.def("Intersects", (bool(AABB::*)(const sp::maths::AABB &))&AABB::Intersects)
				.def("Center", (sp::maths::vec3(AABB::*)())&AABB::Center),

			luabind::class_<Quaternion>("Quaternion"),

			luabind::class_<mat4>("mat4")
				.def(luabind::constructor<>())
				.def("Multiply", (sp::maths::mat4 &(mat4::*)(const sp::maths::mat4 &))&mat4::Multiply)
				.def("Invert", (sp::maths::mat4 &(mat4::*)())&mat4::Invert)
				.def("GetColumn", (sp::maths::vec4(mat4::*)(int))&mat4::GetColumn),

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

			luabind::class_<vec2>("vec2")
				.def(luabind::constructor<>())
				.def("Add", (sp::maths::vec2 &(vec2::*)(const sp::maths::vec2 &))&vec2::Add)
				.def("Subtract", (sp::maths::vec2 &(vec2::*)(const sp::maths::vec2 &))&vec2::Subtract)
				.def("Multiply", (sp::maths::vec2 &(vec2::*)(const sp::maths::vec2 &))&vec2::Multiply)
				.def("Divide", (sp::maths::vec2 &(vec2::*)(const sp::maths::vec2 &))&vec2::Divide)
				.def("Add", (sp::maths::vec2 &(vec2::*)(float))&vec2::Add)
				.def("Subtract", (sp::maths::vec2 &(vec2::*)(float))&vec2::Subtract)
				.def("Multiply", (sp::maths::vec2 &(vec2::*)(float))&vec2::Multiply)
				.def("Divide", (sp::maths::vec2 &(vec2::*)(float))&vec2::Divide)
				.def("Magnitude", (float(vec2::*)())&vec2::Magnitude)
				.def("Normalise", (sp::maths::vec2(vec2::*)())&vec2::Normalise)
				.def("Distance", (float(vec2::*)(const sp::maths::vec2 &))&vec2::Distance)
				.def("Dot", (float(vec2::*)(const sp::maths::vec2 &))&vec2::Dot)
				.def("ToString", (String(vec2::*)())&vec2::ToString),

			luabind::class_<vec2>("vec2"),

			luabind::class_<mat4>("mat4"),

			luabind::class_<vec3>("vec3")
				.def(luabind::constructor<>())
				.def("Add", (sp::maths::vec3 &(vec3::*)(const sp::maths::vec3 &))&vec3::Add)
				.def("Subtract", (sp::maths::vec3 &(vec3::*)(const sp::maths::vec3 &))&vec3::Subtract)
				.def("Multiply", (sp::maths::vec3 &(vec3::*)(const sp::maths::vec3 &))&vec3::Multiply)
				.def("Divide", (sp::maths::vec3 &(vec3::*)(const sp::maths::vec3 &))&vec3::Divide)
				.def("Add", (sp::maths::vec3 &(vec3::*)(float))&vec3::Add)
				.def("Subtract", (sp::maths::vec3 &(vec3::*)(float))&vec3::Subtract)
				.def("Multiply", (sp::maths::vec3 &(vec3::*)(float))&vec3::Multiply)
				.def("Divide", (sp::maths::vec3 &(vec3::*)(float))&vec3::Divide)
				.def("Cross", (sp::maths::vec3(vec3::*)(const sp::maths::vec3 &))&vec3::Cross)
				.def("Dot", (float(vec3::*)(const sp::maths::vec3 &))&vec3::Dot)
				.def("Magnitude", (float(vec3::*)())&vec3::Magnitude)
				.def("Normalize", (sp::maths::vec3(vec3::*)())&vec3::Normalize)
				.def("Distance", (float(vec3::*)(const sp::maths::vec3 &))&vec3::Distance),

			luabind::class_<mat4>("mat4"),

			luabind::class_<vec4>("vec4")
				.def(luabind::constructor<>())
				.def("Add", (sp::maths::vec4 &(vec4::*)(const sp::maths::vec4 &))&vec4::Add)
				.def("Subtract", (sp::maths::vec4 &(vec4::*)(const sp::maths::vec4 &))&vec4::Subtract)
				.def("Multiply", (sp::maths::vec4 &(vec4::*)(const sp::maths::vec4 &))&vec4::Multiply)
				.def("Divide", (sp::maths::vec4 &(vec4::*)(const sp::maths::vec4 &))&vec4::Divide)
				.def("Dot", (float(vec4::*)(const sp::maths::vec4 &))&vec4::Dot),

			luabind::class_<UniformBuffer>("UniformBuffer")
				.def(luabind::constructor<>()),

			luabind::class_<BR2DSystemUniform>("BR2DSystemUniform")
				.def(luabind::constructor<>()),

			luabind::class_<BatchRenderer2D>("BatchRenderer2D")
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

			luabind::class_<DeferredRenderer>("DeferredRenderer")
				.def(luabind::constructor<uint, uint>())
				.def("Init", (void(DeferredRenderer::*)())&DeferredRenderer::Init)
				.def("Begin", (void(DeferredRenderer::*)())&DeferredRenderer::Begin)
				.def("BeginScene", (void(DeferredRenderer::*)(sp::graphics::Camera *))&DeferredRenderer::BeginScene)
				.def("Submit", (void(DeferredRenderer::*)(const sp::graphics::RenderCommand &))&DeferredRenderer::Submit)
				.def("SubmitMesh", (void(DeferredRenderer::*)(sp::graphics::Mesh *, const maths::mat4 &))&DeferredRenderer::SubmitMesh)
				.def("SubmitLightSetup", (void(DeferredRenderer::*)(const sp::graphics::LightSetup &))&DeferredRenderer::SubmitLightSetup)
				.def("EndScene", (void(DeferredRenderer::*)())&DeferredRenderer::EndScene)
				.def("End", (void(DeferredRenderer::*)())&DeferredRenderer::End)
				.def("Present", (void(DeferredRenderer::*)())&DeferredRenderer::Present)
				.def("GetGBuffer", (const sp::graphics::GBuffer &(DeferredRenderer::*)())&DeferredRenderer::GetGBuffer),

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

			luabind::class_<ForwardRenderer>("ForwardRenderer")
				.def(luabind::constructor<>())
				.def("Init", (void(ForwardRenderer::*)())&ForwardRenderer::Init)
				.def("Begin", (void(ForwardRenderer::*)())&ForwardRenderer::Begin)
				.def("BeginScene", (void(ForwardRenderer::*)(sp::graphics::Camera *))&ForwardRenderer::BeginScene)
				.def("Submit", (void(ForwardRenderer::*)(const sp::graphics::RenderCommand &))&ForwardRenderer::Submit)
				.def("SubmitMesh", (void(ForwardRenderer::*)(sp::graphics::Mesh *, const maths::mat4 &))&ForwardRenderer::SubmitMesh)
				.def("SubmitLightSetup", (void(ForwardRenderer::*)(const sp::graphics::LightSetup &))&ForwardRenderer::SubmitLightSetup)
				.def("EndScene", (void(ForwardRenderer::*)())&ForwardRenderer::EndScene)
				.def("End", (void(ForwardRenderer::*)())&ForwardRenderer::End)
				.def("Present", (void(ForwardRenderer::*)())&ForwardRenderer::Present),

			luabind::class_<GBuffer>("GBuffer")
				.def(luabind::constructor<uint, uint>())
				.def("Bind", (void(GBuffer::*)(int32))&GBuffer::Bind)
				.def("SetReadBuffer", (void(GBuffer::*)(sp::graphics::GBuffer::TextureType))&GBuffer::SetReadBuffer)
				.def("GetWidth", (uint(GBuffer::*)())&GBuffer::GetWidth)
				.def("GetHeight", (uint(GBuffer::*)())&GBuffer::GetHeight)
				.def("GetTexture", (uint(GBuffer::*)(uint))&GBuffer::GetTexture),

			luabind::class_<Renderer3D>("Renderer3D"),

			luabind::class_<IRenderable>("IRenderable")
				.def("Render", (void(IRenderable::*)(sp::graphics::Renderer3D &))&IRenderable::Render),

			luabind::class_<Label>("Label")
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

			luabind::class_<MaterialInstance>("MaterialInstance")
				.def(luabind::constructor<sp::graphics::Material *>())
				.def("GetMaterial", (sp::graphics::Material *(MaterialInstance::*)())&MaterialInstance::GetMaterial)
				.def("Bind", (void(MaterialInstance::*)())&MaterialInstance::Bind)
				.def("Unbind", (void(MaterialInstance::*)())&MaterialInstance::Unbind)
				.def("SetUniformData", (void(MaterialInstance::*)(const String &, byte *))&MaterialInstance::SetUniformData)
				.def("SetTexture", (void(MaterialInstance::*)(const String &, API::Texture *))&MaterialInstance::SetTexture)
				.def("GetRenderFlags", (int(MaterialInstance::*)())&MaterialInstance::GetRenderFlags)
				.def("SetRenderFlags", (void(MaterialInstance::*)(int))&MaterialInstance::SetRenderFlags),

			luabind::class_<Renderer3D>("Renderer3D"),

			luabind::class_<Vertex>("Vertex"),

			luabind::class_<Mesh>("Mesh")
				.def(luabind::constructor<API::VertexArray *, API::IndexBuffer *, sp::graphics::MaterialInstance *>())
				.def("SetMaterial", (void(Mesh::*)(sp::graphics::MaterialInstance *))&Mesh::SetMaterial)
				.def("GetMaterialInstance", (sp::graphics::MaterialInstance *(Mesh::*)())&Mesh::GetMaterialInstance)
				.def("Render", (void(Mesh::*)(sp::graphics::Renderer3D &, float, float, float, float, sp::graphics::MaterialInstance *, const maths::vec2 &, const maths::vec2 &, sp::graphics::MaterialInstance *, float, sp::graphics::MaterialInstance *, float, float, const maths::vec3 &, sp::graphics::MaterialInstance *))&Mesh::Render),

			luabind::class_<Model>("Model")
				.def(luabind::constructor<const String &, sp::graphics::MaterialInstance *>())
				.def("Render", (void(Model::*)(sp::graphics::Renderer3D &))&Model::Render)
				.def("GetMesh", (sp::graphics::Mesh *(Model::*)())&Model::GetMesh),

			luabind::class_<PBRMaterial>("PBRMaterial")
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

			luabind::class_<PBRMaterialInstance>("PBRMaterialInstance")
				.def(luabind::constructor<sp::graphics::PBRMaterial *>())
				.def("SetEnviromentMap", (void(PBRMaterialInstance::*)(API::TextureCube *))&PBRMaterialInstance::SetEnviromentMap)
				.def("SetAlbedo", (void(PBRMaterialInstance::*)(const maths::vec4 &))&PBRMaterialInstance::SetAlbedo)
				.def("SetSpecular", (void(PBRMaterialInstance::*)(const maths::vec3 &))&PBRMaterialInstance::SetSpecular)
				.def("SetGloss", (void(PBRMaterialInstance::*)(float))&PBRMaterialInstance::SetGloss)
				.def("UsingNormalMap", (void(PBRMaterialInstance::*)(bool))&PBRMaterialInstance::UsingNormalMap)
				.def("SetAlbedoMap", (void(PBRMaterialInstance::*)(API::Texture2D *))&PBRMaterialInstance::SetAlbedoMap)
				.def("SetSpecularMap", (void(PBRMaterialInstance::*)(API::Texture2D *))&PBRMaterialInstance::SetSpecularMap)
				.def("SetNormalMap", (void(PBRMaterialInstance::*)(API::Texture2D *))&PBRMaterialInstance::SetNormalMap)
				.def("SetGlossMap", (void(PBRMaterialInstance::*)(API::Texture2D *))&PBRMaterialInstance::SetGlossMap),

			luabind::class_<VertexData>("VertexData"),

			luabind::class_<Renderable2D>("Renderable2D")
				.def(luabind::constructor<const maths::vec3 &, const maths::vec2 &, uint>())
				.def("Submit", (void(Renderable2D::*)(sp::graphics::Renderer2D *))&Renderable2D::Submit)
				.def("SetColor", (void(Renderable2D::*)(uint))&Renderable2D::SetColor)
				.def("SetColor", (void(Renderable2D::*)(const maths::vec4 &))&Renderable2D::SetColor)
				.def("GetPosition", (const maths::vec3 &(Renderable2D::*)())&Renderable2D::GetPosition)
				.def("GetSize", (const maths::vec2 &(Renderable2D::*)())&Renderable2D::GetSize)
				.def("GetColor", (const uint(Renderable2D::*)())&Renderable2D::GetColor),

			luabind::class_<RendererUniform>("RendererUniform"),

			luabind::class_<RenderCommand>("RenderCommand"),

			luabind::class_<Renderer>("Renderer"),

			luabind::class_<Renderable2D>("Renderable2D"),

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

			luabind::class_<Renderer3D>("Renderer3D")
				.def("Init", (void(Renderer3D::*)())&Renderer3D::Init)
				.def("Begin", (void(Renderer3D::*)())&Renderer3D::Begin)
				.def("BeginScene", (void(Renderer3D::*)(sp::graphics::Camera *))&Renderer3D::BeginScene)
				.def("Submit", (void(Renderer3D::*)(const sp::graphics::RenderCommand &))&Renderer3D::Submit)
				.def("SubmitMesh", (void(Renderer3D::*)(sp::graphics::Mesh *, const maths::mat4 &))&Renderer3D::SubmitMesh)
				.def("SubmitLightSetup", (void(Renderer3D::*)(const sp::graphics::LightSetup &))&Renderer3D::SubmitLightSetup)
				.def("EndScene", (void(Renderer3D::*)())&Renderer3D::EndScene)
				.def("End", (void(Renderer3D::*)())&Renderer3D::End)
				.def("Present", (void(Renderer3D::*)())&Renderer3D::Present)
				.def("SetScreenBufferSize", (void(Renderer3D::*)(uint, uint))&Renderer3D::SetScreenBufferSize),

			luabind::class_<Renderer3D>("Renderer3D"),

			luabind::class_<Scene>("Scene")
				.def(luabind::constructor<>())
				.def("Add", (void(Scene::*)(entity::Entity *))&Scene::Add)
				.def("PushLightSetup", (void(Scene::*)(sp::graphics::LightSetup *))&Scene::PushLightSetup)
				.def("PopLightSetup", (sp::graphics::LightSetup *(Scene::*)())&Scene::PopLightSetup)
				.def("SetCamera", (void(Scene::*)(sp::graphics::Camera *))&Scene::SetCamera)
				.def("Update", (void(Scene::*)())&Scene::Update)
				.def("Render", (void(Scene::*)(sp::graphics::Renderer3D &))&Scene::Render)
				.def("GetCamera", (sp::graphics::Camera *(Scene::*)())&Scene::GetCamera),

			luabind::class_<Sprite>("Sprite")
				.def(luabind::constructor<API::Texture2D *>())
				.def("SetUV", (void(Sprite::*)(const int))&Sprite::SetUV)
				.def("SetTexture", (void(Sprite::*)(API::Texture2D *))&Sprite::SetTexture),

			luabind::class_<TextureManager>("TextureManager"),

			luabind::class_<Button>("Button")
				.def(luabind::constructor<const String &, const maths::Rectangle &>())
				.def("OnMousePressed", (bool(Button::*)(events::MousePressedEvent &))&Button::OnMousePressed)
				.def("OnMouseReleased", (bool(Button::*)(events::MouseReleasedEvent &))&Button::OnMouseReleased)
				.def("OnMouseMoved", (bool(Button::*)(events::MouseMovedEvent &))&Button::OnMouseMoved)
				.def("OnAction", (void(Button::*)())&Button::OnAction)
				.def("OnUpdate", (void(Button::*)())&Button::OnUpdate)
				.def("OnRender", (void(Button::*)(sp::graphics::Renderer2D &))&Button::OnRender)
				.def("SetLabel", (void(Button::*)(const String &))&Button::SetLabel)
				.def("GetLabel", (const String &(Button::*)())&Button::GetLabel)
				.def("SetFont", (void(Button::*)(sp::graphics::Font *))&Button::SetFont)
				.def("GetFont", (const sp::graphics::Font &(Button::*)())&Button::GetFont)
				.def("SetAction", (void(Button::*)(const int &))&Button::SetAction),

			luabind::class_<Widget>("Widget"),

			luabind::class_<Panel>("Panel")
				.def(luabind::constructor<>())
				.def("Add", (sp::graphics::ui::Widget *(Panel::*)(sp::graphics::ui::Widget *))&Panel::Add)
				.def("Remove", (void(Panel::*)(sp::graphics::ui::Widget *))&Panel::Remove)
				.def("Clear", (void(Panel::*)())&Panel::Clear)
				.def("OnEvent", (void(Panel::*)(events::Event &))&Panel::OnEvent)
				.def("OnMousePressedEvent", (bool(Panel::*)(events::MousePressedEvent &))&Panel::OnMousePressedEvent)
				.def("OnMouseReleasedEvent", (bool(Panel::*)(events::MouseReleasedEvent &))&Panel::OnMouseReleasedEvent)
				.def("OnMouseMovedEvent", (bool(Panel::*)(events::MouseMovedEvent &))&Panel::OnMouseMovedEvent)
				.def("OnUpdate", (void(Panel::*)())&Panel::OnUpdate)
				.def("OnRender", (void(Panel::*)(sp::graphics::Renderer2D &))&Panel::OnRender),

			luabind::class_<Slider>("Slider")
				.def(luabind::constructor<const maths::Rectangle &, bool>())
				.def("OnMousePressed", (bool(Slider::*)(events::MousePressedEvent &))&Slider::OnMousePressed)
				.def("OnMouseReleased", (bool(Slider::*)(events::MouseReleasedEvent &))&Slider::OnMouseReleased)
				.def("OnMouseMoved", (bool(Slider::*)(events::MouseMovedEvent &))&Slider::OnMouseMoved)
				.def("OnUpdate", (void(Slider::*)())&Slider::OnUpdate)
				.def("OnRender", (void(Slider::*)(sp::graphics::Renderer2D &))&Slider::OnRender)
				.def("SetCallback", (void(Slider::*)(const int &))&Slider::SetCallback)
				.def("GetCallback", (const int &(Slider::*)())&Slider::GetCallback)
				.def("GetValue", (float(Slider::*)())&Slider::GetValue)
				.def("SetValue", (void(Slider::*)(float))&Slider::SetValue),

			luabind::class_<Widget>("Widget")
				.def("OnMousePressed", (bool(Widget::*)(events::MousePressedEvent &))&Widget::OnMousePressed)
				.def("OnMouseReleased", (bool(Widget::*)(events::MouseReleasedEvent &))&Widget::OnMouseReleased)
				.def("OnMouseMoved", (bool(Widget::*)(events::MouseMovedEvent &))&Widget::OnMouseMoved)
				.def("OnUpdate", (void(Widget::*)())&Widget::OnUpdate)
				.def("OnRender", (void(Widget::*)(sp::graphics::Renderer2D &))&Widget::OnRender)
				.def("GetBounds", (maths::Rectangle &(Widget::*)())&Widget::GetBounds)
				.def("SetBounds", (void(Widget::*)(const maths::Rectangle &))&Widget::SetBounds)
				.def("IsActive", (bool(Widget::*)())&Widget::IsActive)
				.def("SetActive", (void(Widget::*)(bool))&Widget::SetActive),

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

			luabind::class_<ShaderUniformDeclaration>("ShaderUniformDeclaration")
				.def("GetName", (const String &(ShaderUniformDeclaration::*)())&ShaderUniformDeclaration::GetName)
				.def("GetSize", (uint(ShaderUniformDeclaration::*)())&ShaderUniformDeclaration::GetSize)
				.def("GetCount", (uint(ShaderUniformDeclaration::*)())&ShaderUniformDeclaration::GetCount)
				.def("GetOffset", (uint(ShaderUniformDeclaration::*)())&ShaderUniformDeclaration::GetOffset),

			luabind::class_<ShaderUniformBufferDeclaration>("ShaderUniformBufferDeclaration")
				.def("GetName", (const String &(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetName)
				.def("GetRegister", (uint(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetRegister)
				.def("GetShaderType", (uint(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetShaderType)
				.def("GetSize", (uint(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetSize)
				.def("GetUniformDeclarations", (const int &(ShaderUniformBufferDeclaration::*)())&ShaderUniformBufferDeclaration::GetUniformDeclarations)
				.def("FindUniform", (sp::graphics::API::ShaderUniformDeclaration *(ShaderUniformBufferDeclaration::*)(const String &))&ShaderUniformBufferDeclaration::FindUniform),

			luabind::class_<ShaderStruct>("ShaderStruct")
				.def(luabind::constructor<const String &>())
				.def("AddField", (void(ShaderStruct::*)(sp::graphics::API::ShaderUniformDeclaration *))&ShaderStruct::AddField)
				.def("SetOffset", (void(ShaderStruct::*)(uint))&ShaderStruct::SetOffset)
				.def("GetName", (const String &(ShaderStruct::*)())&ShaderStruct::GetName)
				.def("GetSize", (uint(ShaderStruct::*)())&ShaderStruct::GetSize)
				.def("GetOffset", (uint(ShaderStruct::*)())&ShaderStruct::GetOffset),

			luabind::class_<PostEffects>("PostEffects")
				.def(luabind::constructor<>())
				.def("Push", (void(PostEffects::*)(sp::graphics::PostEffectsPass *))&PostEffects::Push)
				.def("Pop", (void(PostEffects::*)())&PostEffects::Pop)
				.def("RenderPostEffects", (void(PostEffects::*)(sp::graphics::Framebuffer *, sp::graphics::Framebuffer *, API::VertexArray *, API::IndexBuffer *))&PostEffects::RenderPostEffects),

			luabind::class_<PostEffectsPass>("PostEffectsPass")
				.def(luabind::constructor<API::Shader *>())
				.def("RenderPass", (void(PostEffectsPass::*)(sp::graphics::Framebuffer *))&PostEffectsPass::RenderPass),

			luabind::class_<Group>("Group")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("Add", (void(Group::*)(sp::graphics::Renderable2D *))&Group::Add)
				.def("Submit", (void(Group::*)(sp::graphics::Renderer2D *))&Group::Submit)
				.def("GetTransformRef", (maths::mat4 &(Group::*)())&Group::GetTransformRef),

			luabind::class_<Layer>("Layer")
				.def(luabind::constructor<>())
				.def("IsVisible", (bool(Layer::*)())&Layer::IsVisible)
				.def("SetVisible", (void(Layer::*)(bool))&Layer::SetVisible)
				.def("Init", (void(Layer::*)())&Layer::Init)
				.def("OnEvent", (void(Layer::*)(events::Event &))&Layer::OnEvent)
				.def("OnTick", (void(Layer::*)())&Layer::OnTick)
				.def("OnUpdate", (void(Layer::*)())&Layer::OnUpdate)
				.def("OnRender", (void(Layer::*)())&Layer::OnRender),

			luabind::class_<Layer2D>("Layer2D")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("Init", (void(Layer2D::*)())&Layer2D::Init)
				.def("OnInit", (void(Layer2D::*)(sp::graphics::Renderer2D &, sp::graphics::Material &))&Layer2D::OnInit)
				.def("SetMask", (void(Layer2D::*)(const sp::graphics::Mask *))&Layer2D::SetMask)
				.def("Add", (sp::graphics::Renderable2D *(Layer2D::*)(sp::graphics::Renderable2D *))&Layer2D::Add)
				.def("OnRender", (void(Layer2D::*)(sp::graphics::Renderer2D &))&Layer2D::OnRender)
				.def("OnRender", (void(Layer2D::*)())&Layer2D::OnRender),

			luabind::class_<Layer3D>("Layer3D")
				.def(luabind::constructor<sp::graphics::Scene *, sp::graphics::Renderer3D *>())
				.def("Init", (void(Layer3D::*)())&Layer3D::Init)
				.def("OnInit", (void(Layer3D::*)(sp::graphics::Renderer3D &, sp::graphics::Scene &))&Layer3D::OnInit)
				.def("GetScene", (sp::graphics::Scene *(Layer3D::*)())&Layer3D::GetScene)
				.def("OnRender", (void(Layer3D::*)())&Layer3D::OnRender)
				.def("OnRender", (void(Layer3D::*)(sp::graphics::Renderer3D &))&Layer3D::OnRender),

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

			luabind::class_<FPSCamera>("FPSCamera")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("Focus", (void(FPSCamera::*)())&FPSCamera::Focus)
				.def("Update", (void(FPSCamera::*)())&FPSCamera::Update),

			luabind::class_<MayaCamera>("MayaCamera")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("Focus", (void(MayaCamera::*)())&MayaCamera::Focus)
				.def("Update", (void(MayaCamera::*)())&MayaCamera::Update)
				.def("GetDistance", (float(MayaCamera::*)())&MayaCamera::GetDistance)
				.def("SetDistance", (void(MayaCamera::*)(float))&MayaCamera::SetDistance),

			luabind::class_<BufferElement>("BufferElement"),

			luabind::class_<BufferLayout>("BufferLayout"),

			luabind::class_<Context>("Context"),

			luabind::class_<Framebuffer>("Framebuffer")
				.def("Bind", (void(Framebuffer::*)())&Framebuffer::Bind)
				.def("Unbind", (void(Framebuffer::*)())&Framebuffer::Unbind)
				.def("Clear", (void(Framebuffer::*)())&Framebuffer::Clear)
				.def("GetWidth", (uint(Framebuffer::*)())&Framebuffer::GetWidth)
				.def("GetHeight", (uint(Framebuffer::*)())&Framebuffer::GetHeight)
				.def("GetTexture", (API::Texture *(Framebuffer::*)())&Framebuffer::GetTexture),

			luabind::class_<Framebuffer2D>("Framebuffer2D")
				.def("SetClearColor", (void(Framebuffer2D::*)(const maths::vec4 &))&Framebuffer2D::SetClearColor),

			luabind::class_<FramebufferDepth>("FramebufferDepth"),

			luabind::class_<IndexBuffer>("IndexBuffer")
				.def("Bind", (void(IndexBuffer::*)())&IndexBuffer::Bind)
				.def("Unbind", (void(IndexBuffer::*)())&IndexBuffer::Unbind)
				.def("GetCount", (uint(IndexBuffer::*)())&IndexBuffer::GetCount),

			luabind::class_<TextureParameters>("TextureParameters")
				.def(luabind::constructor<>()),

			luabind::class_<TextureLoadOptions>("TextureLoadOptions")
				.def(luabind::constructor<>()),

			luabind::class_<Texture>("Texture")
				.def("Bind", (void(Texture::*)(uint))&Texture::Bind)
				.def("Unbind", (void(Texture::*)(uint))&Texture::Unbind)
				.def("GetName", (const String &(Texture::*)())&Texture::GetName)
				.def("GetFilepath", (const String &(Texture::*)())&Texture::GetFilepath),

			luabind::class_<Texture2D>("Texture2D")
				.def("SetData", (void(Texture2D::*)(const void *))&Texture2D::SetData)
				.def("SetData", (void(Texture2D::*)(uint))&Texture2D::SetData)
				.def("GetWidth", (uint(Texture2D::*)())&Texture2D::GetWidth)
				.def("GetHeight", (uint(Texture2D::*)())&Texture2D::GetHeight),

			luabind::class_<TextureCube>("TextureCube"),

			luabind::class_<TextureDepth>("TextureDepth"),

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

			luabind::class_<Event>("Event")
				.def("IsHandled", (bool(Event::*)())&Event::IsHandled)
				.def("ToString", (String(Event::*)())&Event::ToString),

			luabind::class_<IEventListener>("IEventListener")
				.def("OnEvent", (void(IEventListener::*)(sp::events::Event &))&IEventListener::OnEvent),

			luabind::class_<KeyEvent>("KeyEvent")
				.def(luabind::constructor<int32, Event::Type>())
				.def("GetKeyCode", (int32(KeyEvent::*)())&KeyEvent::GetKeyCode),

			luabind::class_<KeyPressedEvent>("KeyPressedEvent")
				.def(luabind::constructor<int32, int32, int32>())
				.def("GetRepeat", (int32(KeyPressedEvent::*)())&KeyPressedEvent::GetRepeat)
				.def("GetModifiers", (int32(KeyPressedEvent::*)())&KeyPressedEvent::GetModifiers)
				.def("IsModifier", (bool(KeyPressedEvent::*)(int32))&KeyPressedEvent::IsModifier),

			luabind::class_<KeyReleasedEvent>("KeyReleasedEvent")
				.def(luabind::constructor<int32>()),

			luabind::class_<MouseButtonEvent>("MouseButtonEvent")
				.def("GetButton", (const int32(MouseButtonEvent::*)())&MouseButtonEvent::GetButton)
				.def("GetX", (const float(MouseButtonEvent::*)())&MouseButtonEvent::GetX)
				.def("GetY", (const float(MouseButtonEvent::*)())&MouseButtonEvent::GetY)
				.def("GetPosition", (const maths::vec2 &(MouseButtonEvent::*)())&MouseButtonEvent::GetPosition),

			luabind::class_<MousePressedEvent>("MousePressedEvent")
				.def(luabind::constructor<int32, float, float>())
				.def("ToString", (String(MousePressedEvent::*)())&MousePressedEvent::ToString),

			luabind::class_<MouseReleasedEvent>("MouseReleasedEvent")
				.def(luabind::constructor<int32, float, float>()),

			luabind::class_<MouseMovedEvent>("MouseMovedEvent")
				.def(luabind::constructor<float, float, bool>())
				.def("GetX", (const float(MouseMovedEvent::*)())&MouseMovedEvent::GetX)
				.def("GetY", (const float(MouseMovedEvent::*)())&MouseMovedEvent::GetY)
				.def("GetPosition", (const maths::vec2 &(MouseMovedEvent::*)())&MouseMovedEvent::GetPosition)
				.def("IsDragged", (const bool(MouseMovedEvent::*)())&MouseMovedEvent::IsDragged),

			luabind::class_<ResizeWindowEvent>("ResizeWindowEvent")
				.def(luabind::constructor<uint, uint>())
				.def("GetSize", (const maths::tvec2<uint> &(ResizeWindowEvent::*)())&ResizeWindowEvent::GetSize)
				.def("GetWidth", (const uint(ResizeWindowEvent::*)())&ResizeWindowEvent::GetWidth)
				.def("GetHeight", (const uint(ResizeWindowEvent::*)())&ResizeWindowEvent::GetHeight),

			luabind::class_<Entity>("Entity")
				.def(luabind::constructor<>())
				.def("AddComponent", (void(Entity::*)(component::Component *))&Entity::AddComponent),

			luabind::class_<Entity>("Entity"),

			luabind::class_<ComponentType>("ComponentType"),

			luabind::class_<Component>("Component")
				.def("GetEntity", (sp::entity::Entity *(Component::*)())&Component::GetEntity)
				.def("GetType", (sp::entity::component::ComponentType *(Component::*)())&Component::GetType),

			luabind::class_<MeshComponent>("MeshComponent")
				.def(luabind::constructor<graphics::Mesh *>())
				.def("GetType", (sp::entity::component::ComponentType *(MeshComponent::*)())&MeshComponent::GetType),

			luabind::class_<TransformComponent>("TransformComponent")
				.def(luabind::constructor<const maths::mat4 &>())
				.def("GetType", (sp::entity::component::ComponentType *(TransformComponent::*)())&TransformComponent::GetType),

			luabind::class_<DebugLayer>("DebugLayer")
				.def(luabind::constructor<>())
				.def("OnInit", (void(DebugLayer::*)(graphics::Renderer2D &, graphics::Material &))&DebugLayer::OnInit)
				.def("OnTick", (void(DebugLayer::*)())&DebugLayer::OnTick)
				.def("OnUpdate", (void(DebugLayer::*)())&DebugLayer::OnUpdate)
				.def("OnEvent", (void(DebugLayer::*)(events::Event &))&DebugLayer::OnEvent)
				.def("OnMouseMovedEvent", (bool(DebugLayer::*)(events::MouseMovedEvent &))&DebugLayer::OnMouseMovedEvent)
				.def("OnMousePressedEvent", (bool(DebugLayer::*)(events::MousePressedEvent &))&DebugLayer::OnMousePressedEvent)
				.def("OnMouseReleasedEvent", (bool(DebugLayer::*)(events::MouseReleasedEvent &))&DebugLayer::OnMouseReleasedEvent)
				.def("OnKeyPressedEvent", (bool(DebugLayer::*)(events::KeyPressedEvent &))&DebugLayer::OnKeyPressedEvent)
				.def("OnRender", (void(DebugLayer::*)(graphics::Renderer2D &))&DebugLayer::OnRender),

			luabind::class_<IAction>("IAction"),

			luabind::class_<DebugMenuSettings>("DebugMenuSettings"),

			luabind::class_<DebugMenu>("DebugMenu")
				.def("OnActivate", (void(DebugMenu::*)())&DebugMenu::OnActivate)
				.def("OnDeactivate", (void(DebugMenu::*)())&DebugMenu::OnDeactivate)
				.def("EditValues", (void(DebugMenu::*)(const String &, float *, uint, const int *))&DebugMenu::EditValues)
				.def("OnMousePressed", (bool(DebugMenu::*)(events::MousePressedEvent &))&DebugMenu::OnMousePressed)
				.def("OnMouseReleased", (bool(DebugMenu::*)(events::MouseReleasedEvent &))&DebugMenu::OnMouseReleased)
				.def("OnUpdate", (void(DebugMenu::*)())&DebugMenu::OnUpdate)
				.def("OnRender", (void(DebugMenu::*)(graphics::Renderer2D &))&DebugMenu::OnRender),

			luabind::class_<IAction>("IAction")
				.def("OnAction", (void(IAction::*)())&IAction::OnAction)
				.def("ToString", (String(IAction::*)())&IAction::ToString),

			luabind::class_<EmptyAction>("EmptyAction")
				.def(luabind::constructor<const String &>())
				.def("OnAction", (void(EmptyAction::*)())&EmptyAction::OnAction)
				.def("ToString", (String(EmptyAction::*)())&EmptyAction::ToString),

			luabind::class_<IAction>("IAction"),

			luabind::class_<DebugMenuItem>("DebugMenuItem")
				.def(luabind::constructor<sp::debug::IAction *, const maths::Rectangle &>())
				.def("OnUpdate", (void(DebugMenuItem::*)())&DebugMenuItem::OnUpdate)
				.def("GetAction", (const sp::debug::IAction *(DebugMenuItem::*)())&DebugMenuItem::GetAction),

			luabind::class_<DebugMenuSlider>("DebugMenuSlider"),

			luabind::class_<DebugRenderer>("DebugRenderer"),

			luabind::class_<ga_Sound>("ga_Sound"),

			luabind::class_<ga_Handle>("ga_Handle"),

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

			luabind::class_<gau_Manager>("gau_Manager"),

			luabind::class_<ga_Mixer>("ga_Mixer"),

			luabind::class_<SoundManager>("SoundManager"),

			luabind::class_<DebugLayer>("DebugLayer"),

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

			luabind::class_<Window>("Window"),

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

			luabind::class_<Input>("Input"),

			luabind::class_<WindowProperties>("WindowProperties"),

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
				.def("SetEventCallback", (void(Window::*)(const int &))&Window::SetEventCallback)
		];
	}
} }