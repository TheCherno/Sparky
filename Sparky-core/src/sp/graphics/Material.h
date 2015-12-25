#pragma once

#include "sp/sp.h"
#include "sp/Common.h"

#include "sp/Types.h"
#include "sp/graphics/shaders/Shader.h"

namespace sp { namespace graphics {

	class SP_API Material
	{
	private:
		friend class MaterialInstance;

		Shader* m_Shader;
		byte* m_UniformData;
		uint m_UniformDataSize;
	public:
		Material(Shader* shader);
		~Material();

		void Bind() const;
		void Unbind() const;
		void DumpUniformData() const;

		inline Shader* GetShader() const { return m_Shader; }

		template<typename T>
		void SetUniform(const String& name, const T& value)
		{
			const ShaderUniformDeclaration* uniform = GetUniformDeclaration(name);
			if (!uniform)
			{
				SP_ERROR("Could not find uniform '", name, "'!");
				return;
			}
			memcpy(m_UniformData + uniform->GetOffset(), &value, uniform->GetSize());
		}

		template<typename T>
		const T* GetUniform(const String& name) const
		{
			return GetUniform<T>(GetUniformDeclaration(name));
		}

		template<typename T>
		const T* GetUniform(const ShaderUniformDeclaration* uniform) const
		{
			return (T*)&m_UniformData[uniform->GetOffset()];
		}

	private:
		void InitUniformStorage();
		const ShaderUniformDeclaration* GetUniformDeclaration(const String& name) const;
	};

	class SP_API MaterialInstance
	{
	private:
		Material* m_Material;
		byte* m_UniformData;
		uint m_UniformDataSize;
		uint m_SetUniforms;
	public:
		MaterialInstance(Material* material);

		inline Material* GetMaterial() const { return m_Material; }

		void Bind() const;
		void Unbind() const;
		void UnsetUniform(const String& name);

		template<typename T>
		void SetUniform(const String& name, const T& value)
		{
			int index = GetUniformDeclarationIndex(name);
			if (index == -1)
			{
				SP_ERROR("Could not find uniform '", name, "'!");
				return;
			}
			ShaderUniformDeclaration* uniform = m_Material->m_Shader->GetUniformDeclarations()[index];
			memcpy(m_UniformData + uniform->GetOffset(), &value, uniform->GetSize());

			m_SetUniforms |= 1 << index;
		}
	private:
		void InitUniformStorage();
		int GetUniformDeclarationIndex(const String& name) const;
	};

} }
