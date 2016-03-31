#pragma once

#include "sp/String.h"
#include "sp/graphics/shaders/ShaderResource.h"
#include "GLShaderUniform.h"
#include "GLCommon.h"

namespace sp { namespace graphics { namespace API {

	class GLShaderResourceDeclaration : public ShaderResourceDeclaration
	{
	public:
		enum class Type
		{
			NONE, TEXTURE2D, TEXTURECUBE, TEXTURESHADOW
		};
	private:
		friend class GLShader;
	private:
		String m_Name;
		uint m_Register;
		uint m_Count;
		Type m_Type;
	public:
		GLShaderResourceDeclaration(Type type, const String& name, uint count);

		inline const String& GetName() const override { return m_Name; }
		inline uint GetRegister() const override { return m_Register; }
		inline uint GetCount() const override { return m_Count; }

		inline Type GetType() const { return m_Type; }
	public:
		static Type StringToType(const String& type);
		static String TypeToString(Type type);
	};

} } }