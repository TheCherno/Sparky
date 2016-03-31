#pragma once

#include "sp/graphics/API/IndexBuffer.h"

#include "DXCommon.h"

namespace sp { namespace graphics { namespace API {

	class D3DIndexBuffer : public IndexBuffer
	{
	private:
		ID3D11Buffer* m_Handle;
		uint m_Count;
	public:
		D3DIndexBuffer(uint16* data, uint count);
		D3DIndexBuffer(uint* data, uint count);
		~D3DIndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint GetCount() const;
	};


} } }
