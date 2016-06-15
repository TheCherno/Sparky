// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_DETAIL_MAKE_INSTANCE_090310_HPP
# define LUABIND_DETAIL_MAKE_INSTANCE_090310_HPP

# include <luabind/detail/inheritance.hpp>
# include <luabind/detail/object_rep.hpp>

namespace luabind { 
	namespace detail {

		template <class T>
		std::pair<class_id, void*> get_dynamic_class_aux(lua_State* L, T const* p, std::true_type)
		{
			lua_pushliteral(L, "__luabind_class_id_map");
			lua_rawget(L, LUA_REGISTRYINDEX);
			class_id_map& class_ids = *static_cast<class_id_map*>(lua_touserdata(L, -1));
			lua_pop(L, 1);

			return std::make_pair( class_ids.get_local(typeid(*p)), dynamic_cast<void*>(const_cast<T*>(p)) );
		}

		template <class T>
		std::pair<class_id, void*> get_dynamic_class_aux(lua_State*, T const* p, std::false_type)
		{
			return std::make_pair(registered_class<T>::id, (void*)p);
		}

		template <class T>
		std::pair<class_id, void*> get_dynamic_class(lua_State* L, T* p)
		{
			return get_dynamic_class_aux(L, p, std::is_polymorphic<T>());
		}

		template <class T>
		class_rep* get_pointee_class(class_map const& classes, T*)
		{
			return classes.get(registered_class<T>::id);
		}

		template <class P>
		class_rep* get_pointee_class(lua_State* L, P const& p, class_id dynamic_id)
		{
			lua_pushliteral(L, "__luabind_class_map");
			lua_rawget(L, LUA_REGISTRYINDEX);

			class_map const& classes = *static_cast<class_map*>(lua_touserdata(L, -1));

			lua_pop(L, 1);

			class_rep* cls = classes.get(dynamic_id);

			if(!cls) {
				cls = get_pointee_class(classes, get_pointer(p));
			}

			return cls;
		}

		// Create an appropriate instance holder for the given pointer like object.
		template <class P>
		void make_pointer_instance(lua_State* L, P p)
		{
			std::pair<class_id, void*> dynamic = get_dynamic_class(L, get_pointer(p));

			class_rep* cls = get_pointee_class(L, p, dynamic.first);

			if (!cls)
			{
				throw std::runtime_error("Trying to use unregistered class: " + std::string(typeid(P).name()));
			}

			object_rep* instance = push_new_instance(L, cls);

			typedef typename std::remove_reference<P>::type value_type;
			typedef pointer_holder<value_type> holder_type;

			void* storage = instance->allocate(sizeof(holder_type));

			try
			{
				new (storage) holder_type(std::move(p), dynamic.first, dynamic.second);
			}
			catch (...)
			{
				instance->deallocate(storage);
				lua_pop(L, 1);
				throw;
			}

			instance->set_instance(static_cast<holder_type*>(storage));
		}


		template< typename ValueType >
		void make_value_instance(lua_State* L, ValueType&& val, std::true_type /* is smart ptr */)
		{
			std::pair<class_id, void*> dynamic = get_dynamic_class(L, get_pointer(val));
			class_rep* cls = get_pointee_class(L, val, dynamic.first);

			typedef decltype(*get_pointer(val)) pointee_type;

			if(!cls) {
				throw std::runtime_error("Trying to use unregistered class: " + std::string(typeid(pointee_type).name()));
			}

			object_rep* instance = push_new_instance(L, cls);

			typedef typename std::remove_reference<ValueType>::type value_type;
			typedef pointer_like_holder<value_type> holder_type;

			void* storage = instance->allocate(sizeof(holder_type));

			try {
				new (storage) holder_type(L, std::forward<ValueType>(val), dynamic.first, dynamic.second );
			}
			catch(...) {
				instance->deallocate(storage);
				lua_pop(L, 1);
				throw;
			}

			instance->set_instance(static_cast<holder_type*>(storage));
		}

		template< typename ValueType >
		void make_value_instance(lua_State* L, ValueType&& val, std::false_type /* smart ptr */ )
		{
			const auto value_type_id = detail::registered_class<ValueType>::id;
			class_rep* cls = get_pointee_class(L, &val, value_type_id);

			if(!cls) {
				throw std::runtime_error("Trying to use unregistered class: "+ std::string(typeid(ValueType).name()));
			}

			object_rep* instance = push_new_instance(L, cls);
			
			typedef typename std::remove_reference<ValueType>::type value_type;
			typedef value_holder<value_type> holder_type;

			void* storage = instance->allocate(sizeof(holder_type));

			try {
				new (storage) holder_type(L, std::forward<ValueType>(val));
			}
			catch(...) {
				instance->deallocate(storage);
				lua_pop(L, 1);
				throw;
			}

			instance->set_instance(static_cast<holder_type*>(storage));
		}

		template< typename ValueType >
		void make_value_instance(lua_State* L, ValueType&& val)
		{
			make_value_instance(L, std::forward<ValueType>(val), has_get_pointer<ValueType>());
		}

	} // namespace luabind::detail
} 

#endif // LUABIND_DETAIL_MAKE_INSTANCE_090310_HPP

