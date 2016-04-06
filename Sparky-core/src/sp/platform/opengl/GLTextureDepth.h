#pragma once

#include "sp/graphics/API/TextureDepth.h"
#include "GLCommon.h"

namespace sp { namespace graphics { namespace API {

	class GLTextureDepth : public TextureDepth
	{
	private:
		String m_Name;
		uint m_Handle;
		uint m_Width, m_Height;
	public:
		GLTextureDepth(uint width, uint height);
		~GLTextureDepth();

		void Clear() override;

		void Bind(uint slot = 0) const override;
		void Unbind(uint slot = 0) const override;

		void BindForWriting() const override {}
		void UnbindForWriting() const override {}

		inline uint GetHandle() const { return m_Handle; }

		inline const String& GetName() const override { return m_Name; }
		inline const String& GetFilepath() const override { return m_Name; }
	protected:
		void Init();
	};

} } }