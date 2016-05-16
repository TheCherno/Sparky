#pragma once

#include "sp/sp.h"
#include "sp/Common.h"

#include "sp/Types.h"

#include "sp/graphics/API/Texture.h"
#include "sp/graphics/API/Texture2D.h"
#include "sp/graphics/API/TextureCube.h"

#include "sp/graphics/shaders/Shader.h"

namespace sp { namespace graphics {

	class SP_API Material
	{
	public:
		enum class RenderFlags
		{
			NONE				= 0,
			DISABLE_DEPTH_TEST	= BIT(0),
			WIREFRAME			= BIT(1)
		};
	private:
		friend class MaterialInstance;
	protected:
		API::Shader* m_Shader;

		byte* m_VSUserUniformBuffer;
		uint m_VSUserUniformBufferSize;

		byte* m_PSUserUniformBuffer;
		uint m_PSUserUniformBufferSize;

		std::vector<API::Texture*> m_Textures;

		const API::ShaderUniformList* m_VSUserUniforms;
		const API::ShaderUniformList* m_PSUserUniforms;
		const API::ShaderResourceList* m_Resources;

		int m_RenderFlags;
	public:
		explicit Material(API::Shader* shader);
		~Material();

		void Bind();
		void Unbind();
		void SetUniformData(const String& uniform, byte* data);
		void SetTexture(const String& name, API::Texture* texture);

		inline int GetRenderFlags() const { return m_RenderFlags; }
		void SetRenderFlags(int flags) { m_RenderFlags = flags; }
		void SetRenderFlag(Material::RenderFlags flag) { m_RenderFlags |= (int)flag; }

		inline API::Shader* GetShader() { return m_Shader; }

		template<typename T>
		void SetUniform(const String& name, const T& data)
		{
			byte* buffer;
			API::ShaderUniformDeclaration* declaration = FindUniformDeclaration(name, &buffer);
			if (!declaration)
			{
				SP_ERROR("Could not find uniform with name '", name, "'!");
				return;
			}
			memcpy(buffer + declaration->GetOffset(), &data, declaration->GetSize());
		}

		template<typename T>
		const T* GetUniform(const String& name) const
		{
			return GetUniform<T>(GetUniformDeclaration(name));
		}

		template<typename T>
		const T* GetUniform(const API::ShaderUniformDeclaration* uniform) const
		{
			return (T*)&m_UniformData[uniform->GetOffset()];
		}
	protected:
		void AllocateStorage();
		API::ShaderUniformDeclaration* FindUniformDeclaration(const String& name, byte** outBuffer = nullptr);
		API::ShaderResourceDeclaration* FindResourceDeclaration(const String& name);
	};

	class SP_API MaterialInstance
	{
	private:
		Material* m_Material;

		byte* m_VSUserUniformBuffer;
		uint m_VSUserUniformBufferSize;

		byte* m_PSUserUniformBuffer;
		uint m_PSUserUniformBufferSize;

		std::vector<API::Texture*> m_Textures;

		const API::ShaderUniformList* m_VSUserUniforms;
		const API::ShaderUniformList* m_PSUserUniforms;
		const API::ShaderResourceList* m_Resources;

		int m_RenderFlags;
	public:
		explicit MaterialInstance(Material* material);

		inline Material* GetMaterial() const { return m_Material; }

		void Bind();
		void Unbind();
		void SetUniformData(const String& uniform, byte* data);
		void SetTexture(const String& name, API::Texture* texture);

		inline int GetRenderFlags() const { return m_RenderFlags; }
		void SetRenderFlags(int flags) { m_RenderFlags = flags; }
		void SetRenderFlag(Material::RenderFlags flag) { m_RenderFlags |= (int)flag; }

		template<typename T>
		void SetUniform(const String& name, const T& data)
		{
			byte* buffer;
			API::ShaderUniformDeclaration* declaration = FindUniformDeclaration(name, &buffer);
			SP_ASSERT(declaration);
			memcpy(buffer + declaration->GetOffset(), &data, declaration->GetSize());
		}

		template<typename T>
		const T* GetUniform(const String& name) const
		{
			return GetUniform<T>(GetUniformDeclaration(name));
		}
	private:
		void AllocateStorage();
		API::ShaderUniformDeclaration* FindUniformDeclaration(const String& name, byte** outBuffer = nullptr);
		API::ShaderResourceDeclaration* FindResourceDeclaration(const String& name);
	};

} }
