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
		SPARKY_INFO("Dumping uniforms for Material ", (long)this);
		const std::vector<ShaderUniformDeclaration*>& uniforms = m_Shader->GetUniformDeclarations();
		for (uint i = 0; i < uniforms.size(); i++)
		{
			ShaderUniformDeclaration* declaration = uniforms[i];
			switch (declaration->GetType())
			{
			case ShaderUniformDeclaration::Type::MAT4:
				SPARKY_INFO(declaration->GetName(), " = ", GetUniform<maths::mat4>(declaration)->ToString());
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
		: m_Material(material)
	{

	}

} }