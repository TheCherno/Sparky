#pragma once

#include <vector>
#include <map>

#include "String.h"

namespace sp { namespace gen { 

	struct Parameter
	{
		String type;
		String name;

		Parameter(const String& name, const String& type)
			: name(name), type(type) {}
	};

	enum class MethodType
	{
		CONSTRUCTOR, DESTRUCTOR, METHOD
	};

	enum class AccessType
	{
		PUBLIC, PROTECTED, PRIVATE
	};

	struct Method
	{
		MethodType methodType;
		AccessType accessType;

		String name;
		String type;
		bool isStatic;
		std::vector<Parameter> parameters;

		Method(MethodType methodType, AccessType accessType = AccessType::PUBLIC)
			: methodType(methodType), accessType(accessType) {}

		Method(const String& name, const String& type, AccessType accessType = AccessType::PUBLIC, bool isStatic = false)
			: name(name), type(type), methodType(MethodType::METHOD), isStatic(isStatic) {}

		Method(const String& name, const String& type, AccessType accessType = AccessType::PUBLIC, MethodType methodType = MethodType::METHOD, bool isStatic = false)
			: name(name), type(type), accessType(accessType), methodType(methodType), isStatic(isStatic) {}
	};

	struct Class
	{
		AccessType accessType;

		String name;
		String baseClass;
		std::vector<Method> methods;
		std::vector<String> namespaceStack;

		Class(const String& name = "", AccessType accessType = AccessType::PUBLIC)
			: name(name), accessType(accessType) {}

		friend bool operator== (const Class &n1, const Class &n2)
		{
			return n1.name == n2.name;
		}
	};

} }