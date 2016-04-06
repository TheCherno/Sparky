#pragma once

#include "DXCommon.h"
#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/String.h"
#include "sp/graphics/shaders/ShaderUniform.h"
#include <vector>

namespace sp { namespace graphics { namespace API {

	class D3DShaderUniformDeclaration : public ShaderUniformDeclaration
	{
	private:
		friend class D3DShaderUniformBufferDeclaration;
	public:
		enum class Type
		{
			NONE, FLOAT32, VEC2, VEC3, VEC4, MAT3, MAT4, INT32
		};
	private:
		friend class D3DShader;
		friend class ShaderUniformBufferDeclaration;
	private:
		String m_Name;
		uint m_Size;
		uint m_Count;
		uint m_Offset;
		Type m_Type;
		uint m_Register; // TODO: Maybe we can remove this?

		ShaderStruct* m_Struct;
	public:
		D3DShaderUniformDeclaration(Type type, const String& name, uint count = 1);
		D3DShaderUniformDeclaration(ShaderStruct* shaderStruct, const String& name, uint count = 1);

		inline const String& GetName() const override { return m_Name; }
		inline uint GetSize() const override { return m_Size; }
		inline uint GetCount() const override { return m_Count; }
		inline uint GetOffset() const override { return m_Offset; }

		inline Type GetType() const { return m_Type; }
		inline uint GetRegister() const { return m_Register; }
		inline const ShaderStruct* GetStruct() const { return m_Struct; }
	protected:
		inline void SetOffset(uint offset) override { m_Offset = offset; }
	public:
		static Type StringToType(const String& type);
		static String TypeToString(Type type);
		static uint SizeOfUniformType(Type type);
	};

	class D3DShaderUniformBufferDeclaration : public ShaderUniformBufferDeclaration
	{
	private:
		friend class Shader;
	private:
		String m_Name;
		ShaderUniformList m_Uniforms;
		uint m_Register;
		uint m_Size;
		uint m_ShaderType; // 0 = VS, 1 = PS
	public:
		D3DShaderUniformBufferDeclaration(const String& name, uint bufferRegister, uint shaderType);

		void PushUniform(D3DShaderUniformDeclaration* uniform);
		void Align();

		inline const String& GetName() const override { return m_Name; }
		inline uint GetRegister() const override { return m_Register; }
		inline uint GetShaderType() const override { return m_ShaderType; }
		inline uint GetSize() const override { return m_Size; }
		inline const ShaderUniformList& GetUniformDeclarations() const override { return m_Uniforms; }

		ShaderUniformDeclaration* FindUniform(const String& name);
	};

} } }