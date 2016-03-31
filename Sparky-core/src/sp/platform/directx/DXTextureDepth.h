#pragma once

#include "sp/graphics/API/TextureDepth.h"

namespace sp { namespace graphics { namespace API {

	class D3DTextureDepth : public TextureDepth
	{
	private:
		String m_Name;
		uint m_Width, m_Height;
	public:
		D3DTextureDepth(uint width, uint height);
		~D3DTextureDepth();

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		inline const String& GetName() const override { return m_Name; }
		inline const String& GetFilepath() const override { return m_Name; }
	protected:
		void Init();
	};

} } }
