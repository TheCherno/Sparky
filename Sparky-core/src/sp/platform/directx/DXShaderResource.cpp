#include "sp/sp.h"
#include "DXShaderResource.h"

namespace sp { namespace graphics { namespace API { 

	D3DShaderResourceDeclaration::D3DShaderResourceDeclaration(Type type, const String& name, uint count)
		: m_Type(type), m_Name(name), m_Count(count)
	{
	}

	D3DShaderResourceDeclaration::Type D3DShaderResourceDeclaration::StringToType(const String& type)
	{
		if (type == "Texture2D")		return D3DShaderResourceDeclaration::Type::TEXTURE2D;
		if (type == "TextureCube")		return D3DShaderResourceDeclaration::Type::TEXTURECUBE;
		if (type == "SamplerState")		return D3DShaderResourceDeclaration::Type::SAMPLERSTATE;

		return D3DShaderResourceDeclaration::Type::NONE;
	}

	String D3DShaderResourceDeclaration::TypeToString(D3DShaderResourceDeclaration::Type type)
	{
		switch (type)
		{
		case D3DShaderResourceDeclaration::Type::TEXTURE2D:			return "Texture2D";
		case D3DShaderResourceDeclaration::Type::TEXTURECUBE:		return "TextureCube";
		case D3DShaderResourceDeclaration::Type::SAMPLERSTATE:		return "SamplerState";
		}
		return "";
	}

} } }