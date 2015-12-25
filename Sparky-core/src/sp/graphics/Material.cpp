#include "sp/sp.h"
#include "Material.h"

#include <sstream>

namespace sp { namespace graphics {

	Material::Material(Shader* shader)
		: m_Shader(shader)
	{
		InitUniformStorage();
	}

	Material::~Material()
	{
		delete m_Shader;
	}

	void Material::InitUniformStorage()
	{
		m_UniformDataSize = 0;
		const std::vector<ShaderUniformDeclaration*>& uniforms = m_Shader->GetUniformDeclarations();
		for (ShaderUniformDeclaration* uniform : uniforms)
			m_UniformDataSize += uniform->GetSize();

		m_UniformData = new byte[m_UniformDataSize];
		memset(m_UniformData, 0, m_UniformDataSize);
	}

	const ShaderUniformDeclaration* Material::GetUniformDeclaration(const String& name) const
	{
		for (ShaderUniformDeclaration* uniform : m_Shader->GetUniformDeclarations())
		{
			if (uniform->GetName() == name)
				return uniform;
		}
		return nullptr;
	}

	void Material::DumpUniformData() const
	{
		SP_INFO("Dumping uniforms for Material ", (long)this);
		const std::vector<ShaderUniformDeclaration*>& uniforms = m_Shader->GetUniformDeclarations();
		for (uint i = 0; i < uniforms.size(); i++)
		{
			ShaderUniformDeclaration* declaration = uniforms[i];
			switch (declaration->GetType())
			{
			case ShaderUniformDeclaration::Type::MAT4:
				SP_INFO(declaration->GetName(), " = ", GetUniform<maths::mat4>(declaration)->ToString());
				break;
			}
		}
	}

	void Material::Bind() const
	{
		// TODO: Textures. This needs to be resolved by the renderer.

		m_Shader->Bind();
		m_Shader->ResolveAndSetUniforms(m_UniformData, m_UniformDataSize);
	}

	void Material::Unbind() const
	{
		m_Shader->Unbind();
	}

	MaterialInstance::MaterialInstance(Material* material)
		: m_Material(material), m_SetUniforms(0)
	{
		InitUniformStorage();
	}

	void MaterialInstance::InitUniformStorage()
	{
		m_UniformDataSize = 0;
		const std::vector<ShaderUniformDeclaration*>& uniforms = m_Material->m_Shader->GetUniformDeclarations();
		for (ShaderUniformDeclaration* uniform : uniforms)
			m_UniformDataSize += uniform->GetSize();

		m_UniformData = new byte[m_UniformDataSize];
		memset(m_UniformData, 0, m_UniformDataSize);
	}

	int MaterialInstance::GetUniformDeclarationIndex(const String& name) const
	{
		std::vector<ShaderUniformDeclaration*> uniforms = m_Material->m_Shader->GetUniformDeclarations();
		for (uint i = 0; i < uniforms.size(); i++)
		{
			if (uniforms[i]->GetName() == name)
				return i;
		}
		return -1;
	}

	void MaterialInstance::UnsetUniform(const String& name)
	{
		int index = GetUniformDeclarationIndex(name);
		uint mask = !(1 << index);
		m_SetUniforms &= mask;
	}

	void MaterialInstance::Bind() const
	{
		// TODO: Textures. This needs to be resolved by the renderer.

		m_Material->Bind();

		uint overrides = m_SetUniforms;
		uint index = 0;
		while (overrides > 0)
		{
			if (overrides & 1)
				m_Material->m_Shader->ResolveAndSetUniform(index, m_UniformData);

			overrides >>= 1;
			index++;
		}
	}

	void MaterialInstance::Unbind() const
	{
		m_Material->m_Shader->Unbind();
	}

} }