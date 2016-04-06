#pragma once

#include "sp/Common.h"
#include "sp/Types.h"
#include "sp/String.h"
#include "sp/utils/Log.h"

namespace sp { namespace graphics { namespace API {

	class SP_API ShaderUniformDeclaration
	{
	private:
		friend class Shader;
		friend class GLShader;
		friend class DXShader;
		friend class ShaderStruct;
	public:
		virtual const String& GetName() const = 0;
		virtual uint GetSize() const = 0;
		virtual uint GetCount() const = 0;
		virtual uint GetOffset() const = 0;
	protected:
		virtual void SetOffset(uint offset) = 0;
	};

	typedef std::vector<ShaderUniformDeclaration*> ShaderUniformList;

	class ShaderUniformBufferDeclaration
	{
	public:
		virtual const String& GetName() const = 0;
		virtual uint GetRegister() const = 0;
		virtual uint GetShaderType() const = 0;
		virtual uint GetSize() const = 0;
		virtual const ShaderUniformList& GetUniformDeclarations() const = 0;

		virtual ShaderUniformDeclaration* FindUniform(const String& name) = 0;
	};

	typedef std::vector<ShaderUniformBufferDeclaration*> ShaderUniformBufferList;

	class ShaderStruct
	{
	private:
		friend class Shader;
	private:
		String m_Name;
		std::vector<ShaderUniformDeclaration*> m_Fields;
		uint m_Size;
		uint m_Offset;
	public:
		ShaderStruct(const String& name)
			: m_Name(name), m_Size(0), m_Offset(0)
		{
		}

		void AddField(ShaderUniformDeclaration* field)
		{
			m_Size += field->GetSize();
			uint offset = 0;
			if (m_Fields.size())
			{
				ShaderUniformDeclaration* previous = m_Fields.back();
				offset = previous->GetOffset() + previous->GetSize();
			}
			field->SetOffset(offset);
			m_Fields.push_back(field);
		}

		inline void SetOffset(uint offset) { m_Offset = offset; }

		inline const String& GetName() const { return m_Name; }
		inline uint GetSize() const { return m_Size; }
		inline uint GetOffset() const { return m_Offset; }
		inline const std::vector<ShaderUniformDeclaration*>& GetFields() const { return m_Fields; }
	};

	typedef std::vector<ShaderStruct*> ShaderStructList;

} } }
