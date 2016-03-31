#include "sp/sp.h"
#include "PBRMaterial.h"

#include "sp/system/Memory.h"
#include "sp/graphics/shaders/ShaderResource.h"

namespace sp { namespace graphics {

	using namespace API;

	API::Texture2D* PBRMaterial::s_PreintegratedFG = nullptr;

	// TODO: Shader should be selected automatically
	PBRMaterial::PBRMaterial(API::Shader* shader)
		: Material(shader)
	{
		if (s_PreintegratedFG == nullptr)
			s_PreintegratedFG = API::Texture2D::CreateFromFile("PreintegratedFG", "res/pbr/PreintegratedFG.bmp"); // TODO: Embed into engine!

		SetTexture("u_PreintegratedFG", s_PreintegratedFG);
		SetUniform("u_UsingNormalMap", 0.0f);

		// Default parameters for PBR material
		SetUniform("u_UsingAlbedoMap", 0.0f);
		SetUniform("u_AlbedoColor", maths::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		SetUniform("u_SpecularColor", maths::vec3(1.0f, 1.0f, 1.0f));
		SetUniform("u_UsingSpecularMap", 0.0f);

		SetUniform("u_GlossColor", 0.8f);
		SetUniform("u_UsingGlossMap", 0.0f);

		SetUniform("u_UsingNormalMap", 0.0f);
	}

	PBRMaterial::~PBRMaterial()
	{
		spdel m_Shader;
	}

	void PBRMaterial::SetEnviromentMap(API::TextureCube* texture)
	{
		SetTexture("u_EnvironmentMap", texture);
	}

	void PBRMaterial::SetAlbedo(const maths::vec4& color)
	{
		SetUniform("u_AlbedoColor", color);
		SetUniform("u_UsingAlbedoMap", 0.0f);
	}

	void PBRMaterial::SetSpecular(const maths::vec3& color)
	{
		SetUniform("u_SpecularColor", color);
		SetUniform("u_UsingSpecularMap", 0.0f);
	}

	void PBRMaterial::SetGloss(float value)
	{
		SetUniform("u_GlossColor", value);
		SetUniform("u_UsingGlossMap", 0.0f);
	}

	void PBRMaterial::UsingNormalMap(bool value)
	{
		SetUniform("u_UsingNormalMap", value ? 1.0f : 0.0f);
	}

	void PBRMaterial::SetAlbedoMap(API::Texture2D* texture)
	{
		SetTexture("u_AlbedoMap", texture);
		SetUniform("u_UsingAlbedoMap", 1.0f);
	}

	void PBRMaterial::SetSpecularMap(API::Texture2D* texture)
	{
		SetTexture("u_SpecularMap", texture);
		SetUniform("u_UsingSpecularMap", 1.0f);
	}

	void PBRMaterial::SetNormalMap(API::Texture2D* texture)
	{
		SetTexture("u_NormalMap", texture);
		SetUniform("u_UsingNormalMap", 1.0f);
	}

	void PBRMaterial::SetGlossMap(API::Texture2D* texture)
	{
		SetTexture("u_GlossMap", texture);
		SetUniform("u_UsingGlossMap", 1.0f);
	}

	API::Texture* PBRMaterial::GetAlbedoMap()
	{
		ShaderResourceDeclaration* declaration = FindResourceDeclaration("u_AlbedoMap");
		SP_ASSERT(declaration);
		uint slot = declaration->GetRegister();
		return m_Textures.size() > slot ? m_Textures[slot] : nullptr;
	}

	API::Texture* PBRMaterial::GetSpecularMap()
	{
		ShaderResourceDeclaration* declaration = FindResourceDeclaration("u_SpecularMap");
		SP_ASSERT(declaration);
		uint slot = declaration->GetRegister();
		return m_Textures.size() > slot ? m_Textures[slot] : nullptr;
	}

	API::Texture* PBRMaterial::GetNormalMap()
	{
		ShaderResourceDeclaration* declaration = FindResourceDeclaration("u_NormalMap");
		SP_ASSERT(declaration);
		uint slot = declaration->GetRegister();
		return m_Textures.size() > slot ? m_Textures[slot] : nullptr;
	}

	API::Texture* PBRMaterial::GetGlossMap()
	{
		ShaderResourceDeclaration* declaration = FindResourceDeclaration("u_GlossMap");
		SP_ASSERT(declaration);
		uint slot = declaration->GetRegister();
		return m_Textures.size() > slot ? m_Textures[slot] : nullptr;
	}

	PBRMaterialInstance::PBRMaterialInstance(PBRMaterial* material)
		: MaterialInstance(material)
	{
	}

	void PBRMaterialInstance::SetEnviromentMap(API::TextureCube* texture)
	{
		SetTexture("u_EnvironmentMap", texture);
	}

	void PBRMaterialInstance::SetAlbedo(const maths::vec4& color)
	{
		SetUniform("u_AlbedoColor", color);
		SetUniform("u_UsingAlbedoMap", 0.0f);
	}

	void PBRMaterialInstance::SetSpecular(const maths::vec3& color)
	{
		SetUniform("u_SpecularColor", color);
		SetUniform("u_UsingSpecularMap", 0.0f);
	}

	void PBRMaterialInstance::SetGloss(float value)
	{
		SetUniform("u_GlossColor", value);
		SetUniform("u_UsingGlossMap", 0.0f);
	}

	void PBRMaterialInstance::UsingNormalMap(bool value)
	{
		SetUniform("u_UsingNormalMap", value ? 1.0f : 0.0f);
	}

	void PBRMaterialInstance::SetAlbedoMap(API::Texture2D* texture)
	{
		SetTexture("u_AlbedoMap", texture);
		SetUniform("u_UsingAlbedoMap", 1.0f);
	}

	void PBRMaterialInstance::SetSpecularMap(API::Texture2D* texture)
	{
		SetTexture("u_SpecularMap", texture);
		SetUniform("u_UsingSpecularMap", 1.0f);
	}

	void PBRMaterialInstance::SetNormalMap(API::Texture2D* texture)
	{
		SetTexture("u_NormalMap", texture);
		SetUniform("u_UsingNormalMap", 1.0f);
	}

	void PBRMaterialInstance::SetGlossMap(API::Texture2D* texture)
	{
		SetUniform("u_GlossMap", texture);
		SetUniform("u_UsingGlossMap", 1.0f);
	}


} }