#pragma once

#include "sp/Common.h"
#include "sp/Types.h"

namespace sp { namespace graphics { namespace API {

	class ShaderResourceDeclaration
	{
	public:
		virtual const String& GetName() const = 0;
		virtual uint GetRegister() const = 0;
		virtual uint GetCount() const = 0;
	};

	typedef std::vector<ShaderResourceDeclaration*> ShaderResourceList;

} } }