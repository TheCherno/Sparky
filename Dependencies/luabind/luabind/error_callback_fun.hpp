// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef INCLUDED_error_callback_fun_hpp_GUID_1150976a_4348_495f_99ce_9d7edd00a0b8
#define INCLUDED_error_callback_fun_hpp_GUID_1150976a_4348_495f_99ce_9d7edd00a0b8

// Internal Includes
#include <luabind/config.hpp>
#include <luabind/lua_state_fwd.hpp>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace luabind
{
	class type_id;

	typedef void(*error_callback_fun)(lua_State*);
	typedef void(*cast_failed_callback_fun)(lua_State*, type_id const&);
	typedef void(*pcall_callback_fun)(lua_State*);
}

#endif // INCLUDED_error_callback_fun_hpp_GUID_1150976a_4348_495f_99ce_9d7edd00a0b8

