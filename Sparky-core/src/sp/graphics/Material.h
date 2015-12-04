#pragma once

#include <map>

#include <sp/Types.h>
#include <sp/graphics/shaders/Shader.h>

namespace sp { namespace graphics {

	class Material
	{
	private:
		Shader* m_Shader;
		byte* m_UniformData;
		uint m_UniformDataSize;
	public:
		Material(Shader* shader);
		~Material();

		void Bind() const;
		void Unbind() const;
		void DumpUniformData() const;

		template<typename T>
		void SetUniform(const String& name, const T& value)
		{
			const ShaderUniformDeclaration* uniform = GetUniformDeclaration(name);
			if (!uniform)
			{
				SPARKY_ERROR("Could not find uniform '", name, "'!");
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

	class MaterialInstance
	{
	private:
		Material* m_Material;
		byte* m_UniformData;
	public:
		MaterialInstance(Material* material);

		inline Material* GetMaterial() const { return m_Material; }

		void Bind() const;
		void Unbind() const;

		template<typename T>
		void SetUniform(const String& name, const T& value)
		{
			SPARKY_ASSERT(false, "Unknown type");
		}

		template<> void SetUniform<float>(const String& name, const float& value) { }
	private:
		void InitUniformStorage();
	};

} }
