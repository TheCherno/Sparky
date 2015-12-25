#pragma once

#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/String.h"

namespace sp { namespace graphics { 

	class SP_API ShaderUniformDeclaration
	{
	public:
		enum class Type
		{
			NONE, FLOAT32, INT32, VEC2, VEC3, VEC4, MAT3, MAT4, SAMPLER2D
		};
	private:
		friend class Shader;

		Type m_Type;
		String m_Name;
		uint m_Size;
		uint m_Count;
		uint m_Offset;
		const Shader* m_Shader;
		mutable int m_Location;
	public:
		ShaderUniformDeclaration(Type type, const String& name, const Shader* shader, uint count = 1);
		uint GetSize() const;
		int GetLocation() const;
		inline int GetOffset() const { return m_Offset; }
		inline const String& GetName() const { return m_Name; }
		inline Type GetType() const { return m_Type; }
	private:
		uint SizeOfUniformType(Type type);
	};

} }