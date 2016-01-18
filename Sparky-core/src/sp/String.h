#pragma once

#include "sp/sp.h"
#include "sp/Common.h"

typedef unsigned int uint;
typedef std::string String;

namespace sp {

	constexpr auto STRINGFORMAT_BUFFER_SIZE  { 10 * 1024 };

	class SP_API StringFormat
	{
	private:
		static char* s_Buffer;
	public:
		template<typename T>
		static String Hex(const T& input)
		{
			memset(s_Buffer, 0, STRINGFORMAT_BUFFER_SIZE);
			sprintf(s_Buffer, "%02x", input);
			return String(s_Buffer);
		}

		template<typename T>
		static String Hex(const T* input, uint size)
		{
			memset(s_Buffer, 0, STRINGFORMAT_BUFFER_SIZE);
			for (uint i = 0; i < size; i++)
				sprintf(s_Buffer + i * 3, "%02x ", input[i]);
			return String(s_Buffer);
		}
	};

}
