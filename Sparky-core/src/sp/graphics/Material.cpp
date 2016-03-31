#include "sp/sp.h"
#include "Material.h"

#include "sp/graphics/API/Texture2D.h"

#include "sp/system/Memory.h"
#include "sp/graphics/shaders/ShaderResource.h"

#include <sstream>

namespace sp { namespace graphics {

	using namespace API;

	Material::Material(API::Shader* shader)
		: m_Shader(shader)
	{
		AllocateStorage();
		m_Resources = &shader->GetResources();
	}

	Material::~Material()
	{
		spdel[] m_VSUserUniformBuffer;
		spdel[] m_PSUserUniformBuffer;
	}

	void Material::AllocateStorage()
	{
		m_VSUserUniformBuffer = nullptr;
		m_VSUserUniformBufferSize = 0;

		m_PSUserUniformBuffer = nullptr;
		m_PSUserUniformBufferSize = 0;

		m_VSUserUniforms = nullptr;
		m_PSUserUniforms = nullptr;

		const ShaderUniformBufferDeclaration* vsBuffer = m_Shader->GetVSUserUniformBuffer();
		if (vsBuffer)
		{
			m_VSUserUniformBufferSize = vsBuffer->GetSize();
			m_VSUserUniformBuffer = spnew byte[m_VSUserUniformBufferSize];
			memset(m_VSUserUniformBuffer, 0, m_VSUserUniformBufferSize);
			m_VSUserUniforms = &vsBuffer->GetUniformDeclarations();
		}

		const ShaderUniformBufferDeclaration* psBuffer = m_Shader->GetPSUserUniformBuffer();
		if (psBuffer)
		{
			m_PSUserUniformBufferSize = psBuffer->GetSize();
			m_PSUserUniformBuffer = spnew byte[m_PSUserUniformBufferSize];
			memset(m_PSUserUniformBuffer, 0, m_PSUserUniformBufferSize);
			m_PSUserUniforms = &psBuffer->GetUniformDeclarations();
		}
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		// TODO: Don't do this if a MaterialInstance is being used
		if (m_VSUserUniformBuffer)
			m_Shader->SetVSUserUniformBuffer(m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
		if (m_PSUserUniformBuffer)
			m_Shader->SetPSUserUniformBuffer(m_PSUserUniformBuffer, m_PSUserUniformBufferSize);

		for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}

	void Material::Unbind()
	{
		for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Unbind(i);
		}
	}

	void Material::SetUniformData(const String& uniform, byte* data)
	{
		byte* buffer;
		ShaderUniformDeclaration* declaration = FindUniformDeclaration(uniform, &buffer);
		memcpy(buffer + declaration->GetOffset(), data, declaration->GetSize());
	}

	void Material::SetTexture(const String& name, Texture* texture)
	{
		ShaderResourceDeclaration* declaration = FindResourceDeclaration(name);
		SP_ASSERT(declaration);
		uint slot = declaration->GetRegister();
		if (m_Textures.size() <= slot)
			m_Textures.resize(slot + 1);
		m_Textures[slot] = texture;
	}

	ShaderUniformDeclaration* Material::FindUniformDeclaration(const String& name, byte** outBuffer)
	{
		if (m_VSUserUniforms)
		{
			for (ShaderUniformDeclaration* uniform : *m_VSUserUniforms)
			{
				if (uniform->GetName() == name)
				{
					*outBuffer = m_VSUserUniformBuffer;
					return uniform;
				}
			}
		}
		if (m_PSUserUniforms)
		{
			for (ShaderUniformDeclaration* uniform : *m_PSUserUniforms)
			{
				if (uniform->GetName() == name)
				{
					*outBuffer = m_PSUserUniformBuffer;
					return uniform;
				}
			}
		}
		return nullptr;
	}

	ShaderResourceDeclaration* Material::FindResourceDeclaration(const String& name)
	{
		for (ShaderResourceDeclaration* resource : *m_Resources)
		{
			if (resource->GetName() == name)
				return resource;
		}
		return nullptr;
	}


	MaterialInstance::MaterialInstance(Material* material)
		: m_Material(material)
	{
		AllocateStorage();
		memcpy(m_VSUserUniformBuffer, m_Material->m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
		memcpy(m_PSUserUniformBuffer, m_Material->m_PSUserUniformBuffer, m_PSUserUniformBufferSize);

		m_Resources = &m_Material->GetShader()->GetResources();
		m_RenderFlags = material->m_RenderFlags;
	}

	void MaterialInstance::AllocateStorage()
	{
		const ShaderUniformBufferDeclaration* vsBuffer = m_Material->m_Shader->GetVSUserUniformBuffer();
		if (vsBuffer)
		{
			m_VSUserUniformBufferSize = vsBuffer->GetSize();
			m_VSUserUniformBuffer = new byte[m_VSUserUniformBufferSize];
			m_VSUserUniforms = &vsBuffer->GetUniformDeclarations();
		}

		const ShaderUniformBufferDeclaration* psBuffer = m_Material->m_Shader->GetPSUserUniformBuffer();
		if (psBuffer)
		{
			m_PSUserUniformBufferSize = psBuffer->GetSize();
			m_PSUserUniformBuffer = new byte[m_PSUserUniformBufferSize];
			m_PSUserUniforms = &psBuffer->GetUniformDeclarations();
		}
	}

	void MaterialInstance::Bind()
	{
		m_Material->Bind();

		if (m_VSUserUniformBuffer)
			m_Material->m_Shader->SetVSUserUniformBuffer(m_VSUserUniformBuffer, m_VSUserUniformBufferSize);
		if (m_PSUserUniformBuffer)
			m_Material->m_Shader->SetPSUserUniformBuffer(m_PSUserUniformBuffer, m_PSUserUniformBufferSize);

		for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}

	void MaterialInstance::Unbind()
	{
		m_Material->Unbind();

		for (uint i = 0; i < m_Textures.size(); i++)
		{
			Texture* texture = m_Textures[i];
			if (texture)
				texture->Unbind(i);
		}
	}

	void MaterialInstance::SetUniformData(const String& uniform, byte* data)
	{
		byte* buffer;
		ShaderUniformDeclaration* declaration = FindUniformDeclaration(uniform, &buffer);
		SP_ASSERT(buffer);
		memcpy(buffer + declaration->GetOffset(), data, declaration->GetSize());
	}

	void MaterialInstance::SetTexture(const String& name, Texture* texture)
	{
		ShaderResourceDeclaration* declaration = FindResourceDeclaration(name);
		uint slot = declaration->GetRegister();
		if (m_Textures.size() <= slot)
			m_Textures.resize(slot + 1);
		m_Textures[slot] = texture;
	}

	ShaderUniformDeclaration* MaterialInstance::FindUniformDeclaration(const String& name, byte** outBuffer)
	{
		if (m_VSUserUniforms)
		{
			for (ShaderUniformDeclaration* uniform : *m_VSUserUniforms)
			{
				if (uniform->GetName() == name)
				{
					*outBuffer = m_VSUserUniformBuffer;
					return uniform;
				}
			}
		}
		if (m_PSUserUniforms)
		{
			for (ShaderUniformDeclaration* uniform : *m_PSUserUniforms)
			{
				if (uniform->GetName() == name)
				{
					*outBuffer = m_PSUserUniformBuffer;
					return uniform;
				}
			}
		}
		return nullptr;
	}

	ShaderResourceDeclaration* MaterialInstance::FindResourceDeclaration(const String& name)
	{
		for (ShaderResourceDeclaration* resource : *m_Resources)
		{
			if (resource->GetName() == name)
				return resource;
		}
		return nullptr;
	}


} }