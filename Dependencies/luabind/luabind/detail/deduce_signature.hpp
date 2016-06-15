// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# ifndef LUABIND_DEDUCE_SIGNATURE_080911_HPP
# define LUABIND_DEDUCE_SIGNATURE_080911_HPP

#include <luabind/detail/meta.hpp>
#include <luabind/detail/most_derived.hpp>

namespace luabind {

	namespace detail {

		template< typename, typename > struct tagged_function;

		template< typename T, typename WrappedType = detail::null_type >
		struct call_types;

		template< typename R, typename... Args, typename WrappedType >
		struct call_types < R(*)(Args...), WrappedType >
		{
			typedef meta::type_list< R, Args... > signature_type;
		};

		template< typename R, typename Class, typename... Args >
		struct call_types < R(Class::*)(Args...), detail::null_type >
		{
			typedef meta::type_list< R, Class&, Args... > signature_type;
		};

		template< typename R, typename Class, typename... Args >
		struct call_types < R(Class::*)(Args...) const, detail::null_type >
		{
			typedef meta::type_list< R, Class const&, Args... > signature_type;
		};

		template< typename R, typename Class, typename... Args, class WrappedType >
		struct call_types < R(Class::*)(Args...), WrappedType >
		{
			typedef meta::type_list< R, typename most_derived<Class, WrappedType>::type&, Args... > signature_type;
		};

		template< typename R, typename Class, typename... Args, class WrappedType >
		struct call_types < R(Class::*)(Args...) const, WrappedType >
		{
			typedef meta::type_list< R, typename most_derived<Class, WrappedType>::type const&, Args... > signature_type;
		};

		template< typename Signature, typename F, class WrappedType >
		struct call_types< tagged_function< Signature, F >, WrappedType >
		{
			typedef Signature signature_type;
		};

	}	// namespace detail

} // namespace luabind

# endif // LUABIND_DEDUCE_SIGNATURE_080911_HPP

