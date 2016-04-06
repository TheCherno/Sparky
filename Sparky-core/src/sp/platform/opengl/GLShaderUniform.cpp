#include "sp/sp.h"
#include "GLShaderUniform.h"

#include "sp/graphics/shaders/Shader.h"

namespace sp { namespace graphics { namespace API {

	GLShaderUniformDeclaration::GLShaderUniformDeclaration(Type type, const String& name, uint count)
		: m_Type(type), m_Struct(nullptr)
	{
		m_Name = name;
		m_Count = count;
		m_Size = SizeOfUniformType(type) * count;
	}

	GLShaderUniformDeclaration::GLShaderUniformDeclaration(ShaderStruct* uniformStruct, const String& name, uint count)
		: m_Struct(uniformStruct), m_Type(GLShaderUniformDeclaration::Type::STRUCT)
	{
		m_Name = name;
		m_Count = count;
		m_Size = m_Struct->GetSize() * count;
	}

	void GLShaderUniformDeclaration::SetOffset(uint offset)
	{
		if (m_Type == GLShaderUniformDeclaration::Type::STRUCT)
			m_Struct->SetOffset(offset);

		m_Offset = offset;
	}

	uint GLShaderUniformDeclaration::SizeOfUniformType(Type type)
	{
		switch (type)
		{
			case GLShaderUniformDeclaration::Type::INT32:				return 4;
			case GLShaderUniformDeclaration::Type::FLOAT32:				return 4;
			case GLShaderUniformDeclaration::Type::VEC2:				return 4 * 2;
			case GLShaderUniformDeclaration::Type::VEC3:				return 4 * 3;
			case GLShaderUniformDeclaration::Type::VEC4:				return 4 * 4;
			case GLShaderUniformDeclaration::Type::MAT3:				return 4 * 3 * 3;
			case GLShaderUniformDeclaration::Type::MAT4:				return 4 * 4 * 4;
		}
		return 0;
	}

	GLShaderUniformDeclaration::Type GLShaderUniformDeclaration::StringToType(const String& type)
	{
		if (type == "int32")		return Type::INT32;
		if (type == "float")	return Type::FLOAT32;
		if (type == "vec2")		return Type::VEC2;
		if (type == "vec3")		return Type::VEC3;
		if (type == "vec4")		return Type::VEC4;
		if (type == "mat3")		return Type::MAT3;
		if (type == "mat4")		return Type::MAT4;

		return Type::NONE;
	}

	String GLShaderUniformDeclaration::TypeToString(Type type)
	{
		switch (type)
		{
			case GLShaderUniformDeclaration::Type::INT32:		return "int32";
			case GLShaderUniformDeclaration::Type::FLOAT32:		return "float";
			case GLShaderUniformDeclaration::Type::VEC2:		return "vec2";
			case GLShaderUniformDeclaration::Type::VEC3:		return "vec3";
			case GLShaderUniformDeclaration::Type::VEC4:		return "vec4";
			case GLShaderUniformDeclaration::Type::MAT3:		return "mat3";
			case GLShaderUniformDeclaration::Type::MAT4:		return "mat4";
		}
		return "Invalid Type";
	}

	GLShaderUniformBufferDeclaration::GLShaderUniformBufferDeclaration(const String& name, uint shaderType)
		: m_Name(name), m_ShaderType(shaderType), m_Size(0), m_Register(0)
	{
	}

	void GLShaderUniformBufferDeclaration::PushUniform(GLShaderUniformDeclaration* uniform)
	{
		uint offset = 0;
		if (m_Uniforms.size())
		{
			GLShaderUniformDeclaration* previous = (GLShaderUniformDeclaration*)m_Uniforms.back();
			offset = previous->m_Offset + previous->m_Size;
		}
		uniform->SetOffset(offset);
		m_Size += uniform->GetSize();
		m_Uniforms.push_back(uniform);
	}

	ShaderUniformDeclaration* GLShaderUniformBufferDeclaration::FindUniform(const String& name)
	{
		for (ShaderUniformDeclaration* uniform : m_Uniforms)
		{
			if (uniform->GetName() == name)
				return uniform;
		}
		return nullptr;
	}

} } }