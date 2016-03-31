#include "sp/sp.h"
#include "DXShaderUniform.h"

namespace sp { namespace graphics { namespace API {

	#define BUFFER_ALIGNMENT 16

	D3DShaderUniformDeclaration::D3DShaderUniformDeclaration(Type type, const String& name, uint count)
		: m_Type(type)
	{
		m_Size = SizeOfUniformType(type) * count;
		m_Name = name;
		m_Count = count;
		m_Offset = 0;
	}

	D3DShaderUniformDeclaration::D3DShaderUniformDeclaration(ShaderStruct* shaderStruct, const String& name, uint count)
		: m_Type(Type::NONE), m_Struct(shaderStruct)
	{
		m_Size = m_Struct->GetSize();
		m_Name = name;
		m_Count = count;
		m_Offset = 0;
	}

	D3DShaderUniformDeclaration::Type D3DShaderUniformDeclaration::StringToType(const String& type)
	{
		if (type == "float")		return D3DShaderUniformDeclaration::Type::FLOAT32;
		if (type == "float2")		return D3DShaderUniformDeclaration::Type::VEC2;
		if (type == "float3")		return D3DShaderUniformDeclaration::Type::VEC3;
		if (type == "float4")		return D3DShaderUniformDeclaration::Type::VEC4;
		if (type == "float3x3")		return D3DShaderUniformDeclaration::Type::MAT3;
		if (type == "float4x4")		return D3DShaderUniformDeclaration::Type::MAT4;
		if (type == "int32")			return D3DShaderUniformDeclaration::Type::FLOAT32;

		return D3DShaderUniformDeclaration::Type::NONE;
	}

	String D3DShaderUniformDeclaration::TypeToString(D3DShaderUniformDeclaration::Type type)
	{
		switch (type)
		{
			case D3DShaderUniformDeclaration::Type::FLOAT32:	return "float";
			case D3DShaderUniformDeclaration::Type::VEC2:		return "float2";
			case D3DShaderUniformDeclaration::Type::VEC3:		return "float3";
			case D3DShaderUniformDeclaration::Type::VEC4:		return "float4";
			case D3DShaderUniformDeclaration::Type::MAT3:		return "float3x3";
			case D3DShaderUniformDeclaration::Type::MAT4:		return "float4x4";
			case D3DShaderUniformDeclaration::Type::INT32:		return "int32";
		}
		return "";
	}

	uint D3DShaderUniformDeclaration::SizeOfUniformType(Type type)
	{
		switch (type)
		{
			case D3DShaderUniformDeclaration::Type::FLOAT32:	return 4;
			case D3DShaderUniformDeclaration::Type::INT32:		return 4;
			case D3DShaderUniformDeclaration::Type::VEC2:		return 4 * 2;
			case D3DShaderUniformDeclaration::Type::VEC3:		return 4 * 3;
			case D3DShaderUniformDeclaration::Type::VEC4:		return 4 * 4;
			case D3DShaderUniformDeclaration::Type::MAT3:		return 4 * 3 * 3;
			case D3DShaderUniformDeclaration::Type::MAT4:		return 4 * 4 * 4;
		}
		SP_ASSERT(false);
		return 0;
	}

	D3DShaderUniformBufferDeclaration::D3DShaderUniformBufferDeclaration(const String& name, uint bufferRegister, uint shaderType)
		: m_Name(name), m_Register(bufferRegister), m_ShaderType(shaderType), m_Size(0)
	{
	}

	void D3DShaderUniformBufferDeclaration::PushUniform(D3DShaderUniformDeclaration* uniform)
	{
		uint offset = 0;
		if (m_Uniforms.size())
		{
			D3DShaderUniformDeclaration* previous = (D3DShaderUniformDeclaration*)m_Uniforms.back();
			offset = previous->m_Offset + previous->m_Size;
		}
		uniform->m_Offset = offset;
		m_Size += uniform->GetSize();
		m_Uniforms.push_back(uniform);
	}

	void D3DShaderUniformBufferDeclaration::Align()
	{
		m_Size = (m_Size + (BUFFER_ALIGNMENT - 1)) / BUFFER_ALIGNMENT * BUFFER_ALIGNMENT;
	}

	ShaderUniformDeclaration* D3DShaderUniformBufferDeclaration::FindUniform(const String& name)
	{
		for (ShaderUniformDeclaration* uniform : m_Uniforms)
		{
			if (uniform->GetName() == name)
				return uniform;
		}
		return nullptr;
	}

} } }