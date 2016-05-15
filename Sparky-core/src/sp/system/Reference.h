// This is a modified version of the /core/ref.hpp file located in Boost

#pragma once

namespace sp { namespace internal {

	template <typename T> T* addressof(T& v)
	{
		return reinterpret_cast<T*>(
			&const_cast<char&>(reinterpret_cast<const volatile char &>(v)));
	}

	template<class T> class reference_wrapper
	{
	public:
		typedef T type;

		explicit reference_wrapper(T& t) : t_(addressof(t)) {}

		operator T& () const { return *t_; }

		T& get() const { return *t_; }

		T* get_pointer() const { return t_; }

	private:

		T* t_;
	};

	template<class T> reference_wrapper<T> ref(T & t)
	{
		return reference_wrapper<T>(t);
	}

	template<class T> reference_wrapper<T const> cref(T const & t)
	{
		return reference_wrapper<T const>(t);
	}

} }