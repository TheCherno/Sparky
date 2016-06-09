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

	struct Method
	{
		MethodType methodType;

		String name;
		String type;
		bool isStatic;
		std::vector<Parameter> parameters;

		Method(MethodType methodType)
			: methodType(methodType) {}

		Method(const String& name, const String& type, bool isStatic = false)
			: name(name), type(type), methodType(MethodType::METHOD), isStatic(isStatic) {}

		Method(const String& name, const String& type, MethodType methodType = MethodType::METHOD, bool isStatic = false)
			: name(name), type(type), methodType(methodType), isStatic(isStatic) {}
	};

	struct Class
	{
		String name;
		String baseClass;
		std::vector<Method> methods;
		std::vector<String> namespaceStack;

		Class(const String& name)
			: name(name) {}
	};

} }