#include "sp/sp.h"
#include "GLShaderResource.h"

namespace sp { namespace graphics { namespace API {

	GLShaderResourceDeclaration::GLShaderResourceDeclaration(Type type, const String& name, uint count)
		: m_Type(type), m_Name(name), m_Count(count)
	{
		m_Name = name;
		m_Count = count;
	}

	GLShaderResourceDeclaration::Type GLShaderResourceDeclaration::StringToType(const String& type)
	{
		if (type == "sampler2D")		return Type::TEXTURE2D;
		if (type == "samplerCube")		return Type::TEXTURECUBE;
		if (type == "samplerShadow")	return Type::TEXTURESHADOW;

		return Type::NONE;
	}

	String GLShaderResourceDeclaration::TypeToString(Type type)
	{
		switch (type)
		{
			case Type::TEXTURE2D:	return "sampler2D";
			case Type::TEXTURECUBE:		return "samplerCube";
			case Type::TEXTURESHADOW:	return "samplerShadow";
		}
		return "Invalid Type";
	}

} } }