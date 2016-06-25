#include "sp.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include <clang-c/Index.h>

#include "utils/CTypes.h"
#include "utils/String.h"

#include "Writer.h"

#include <stack>
#include <Windows.h>

using namespace sp::gen;

std::map<String, Class> classes;
std::vector<String> namespaceStack;

Class* currentClass = nullptr;
Method* currentMethod = nullptr;
Class* parentClass = nullptr;

namespace sp { namespace cligen {
	std::map<String, String> g_TypeMappings;
} }

CXChildVisitResult traverse(CXCursor cursor, CXCursor parent, CXClientData data)
{
	CXString cursorSpelling = clang_getCursorSpelling(cursor);
	CXString cursorKind = clang_getCursorKindSpelling(cursor.kind);
	String type = String(clang_getCString(cursorKind));
	String name = clang_getCString(cursorSpelling);

 	if (!clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
 		return CXChildVisit_Recurse;

	if (type == "Namespace")
	{
		namespaceStack.push_back(name);
	}
	else if (type == "ClassDecl")
	{
		if (!classes.empty()) {
			if (parentClass != nullptr) {
				if (currentClass->methods.size() == 0) {
					classes.erase(currentClass->name);
				}
			}
		}
		AccessType accessType;
		CX_CXXAccessSpecifier intAccessType = clang_getCXXAccessSpecifier(cursor);
		switch (intAccessType)
		{
		case CX_CXXInvalidAccessSpecifier:
			accessType = AccessType::PUBLIC;
			break;
		case CX_CXXPublic:
			accessType = AccessType::PUBLIC;
			break;
		case CX_CXXProtected:
			accessType = AccessType::PROTECTED;
			break;
		case CX_CXXPrivate:
			accessType = AccessType::PRIVATE;
			break;
		}
		classes[name] = Class(name, accessType);
		currentClass = &classes[name];
		currentClass->namespaceStack = namespaceStack;
		namespaceStack.clear();
	}
	else if (type == "StructDecl")
	{
		if (!classes.empty()) {
			if (parentClass != nullptr) {
				if (currentClass->methods.size() == 0) {
					classes.erase(currentClass->name);
				}
			}
		}
		AccessType accessType;
		CX_CXXAccessSpecifier intAccessType = clang_getCXXAccessSpecifier(cursor);
		switch (intAccessType)
		{
		case CX_CXXInvalidAccessSpecifier:
			accessType = AccessType::PUBLIC;
			break;
		case CX_CXXPublic:
			accessType = AccessType::PUBLIC;
			break;
		case CX_CXXProtected:
			accessType = AccessType::PROTECTED;
			break;
		case CX_CXXPrivate:
			accessType = AccessType::PRIVATE;
			break;
		}
		classes[name] = Class(name, accessType);
		currentClass = &classes[name];
		currentClass->namespaceStack = namespaceStack;
		namespaceStack.clear();
	}
	else if (type == "CXXConstructor")
	{
		if (currentClass)
		{
			SP_ASSERT(currentClass);

			AccessType accessType;
			CX_CXXAccessSpecifier intAccessType = clang_getCXXAccessSpecifier(cursor);
			switch (intAccessType)
			{
			case CX_CXXInvalidAccessSpecifier:
				break;
			case CX_CXXPublic:
				accessType = AccessType::PUBLIC;
				break;
			case CX_CXXProtected:
				accessType = AccessType::PROTECTED;
				break;
			case CX_CXXPrivate:
				accessType = AccessType::PRIVATE;
				break;
			}

			currentClass->methods.push_back(Method(MethodType::CONSTRUCTOR, accessType));
			currentMethod = &currentClass->methods.back();
		}
	}
	else if (type == "CXXMethod")
	{
		if (currentClass)
		{
			SP_ASSERT(currentClass);
			CXString resultType = clang_getTypeSpelling(clang_getResultType(clang_getCursorType(cursor)));
			bool isStatic = clang_CXXMethod_isStatic(cursor);

			AccessType accessType;
			CX_CXXAccessSpecifier intAccessType = clang_getCXXAccessSpecifier(cursor);
			switch (intAccessType)
			{
			case CX_CXXInvalidAccessSpecifier:
				break;
			case CX_CXXPublic:
				accessType = AccessType::PUBLIC;
				break;
			case CX_CXXProtected:
				accessType = AccessType::PROTECTED;
				break;
			case CX_CXXPrivate:
				accessType = AccessType::PRIVATE;
				break;
			}

			currentClass->methods.push_back(Method(name, String(clang_getCString(resultType)), accessType, MethodType::METHOD, isStatic));
			currentMethod = &currentClass->methods.back();
			clang_disposeString(resultType);
		}
	}
	else if (type == "ParmDecl")
	{
		if (currentClass && currentMethod)
		{
			SP_ASSERT(currentClass && currentMethod);
			CXString paramType = clang_getTypeSpelling(clang_getCursorType(cursor));
			currentClass->methods.back().parameters.push_back(Parameter(name, clang_getCString(paramType)));
			clang_disposeString(paramType);
		}
	}
	else if (type == "TemplateArgument")
	{
		if (currentClass && currentMethod)
		{
			SP_ASSERT(currentClass && currentMethod);
			currentClass->methods.erase(currentClass->methods.begin() + currentClass->methods.size());
		}
	}
	else if (type == "C++ base class specifier")
	{
		SP_ASSERT(currentClass);
		std::string className = SplitString(SplitString(name, " ").back(), "::").back();
		currentClass->baseClass = className;

		if (classes.find(className) == classes.end()) {
			classes[className] = Class(className, AccessType::PUBLIC);
			parentClass = &classes[className];
		}
	}

	clang_disposeString(cursorKind);
	return CXChildVisit_Recurse;
}

static std::vector<String> GetAllFiles(String path, const String& mask)
{
	std::vector<String> results;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	String spec;
	std::stack<String> directories;

	directories.push(path);

	while (!directories.empty()) {
		path = directories.top();
		spec = path + "/" + mask;
		directories.pop();

		hFind = FindFirstFile(spec.c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE) {
			return results;
		}

		do {
			if (strcmp(ffd.cFileName, ".") != 0 &&
				strcmp(ffd.cFileName, "..") != 0) {
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					directories.push(path + "/" + ffd.cFileName);
				}
				else {
					if (path.find("platform") != std::string::npos) continue;
					if (path.find("scripting") != std::string::npos) continue;

					results.push_back(path + "/" + ffd.cFileName);
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		if (GetLastError() != ERROR_NO_MORE_FILES) {
			FindClose(hFind);
			return results;
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	return results;
}

String GetFileName(const String& path)
{
	std::vector<String> result = SplitString(path, "/");
	return result.back();
}

void AddToVector(std::vector<Class>* v, Class c) {
	auto res = std::find(std::begin(*v), std::end(*v), c);
	if (res != std::end(*v)) 
	{
		
	}
	else {
		v->push_back(c);
	}
}

std::vector<Class> SortClasses(std::map<String, Class> classes) {
	String ignoreClasses[] = { "VFS", "Event", "Panel", "Button", "Widget", "MaterialInstance", "Action", "BufferLayout", "GBuffer", "DeferredRenderer", "Renderer3D", "IEventListener", "Layer" };
	
	std::vector<Class> sort;
	for (std::map<String, Class>::iterator iter = classes.begin(); iter != classes.end(); ++iter)
	{
		Class c = iter->second;
		bool ignore = false;
		for (int i = 0; i < sizeof(ignoreClasses) / sizeof(String); i++) {
			if (c.name.find(ignoreClasses[i]) != std::string::npos) ignore = true;
			if (c.baseClass.find(ignoreClasses[i]) != std::string::npos) ignore = true;
		}
		if (ignore) continue;

		AddToVector(&sort, classes[c.baseClass]);
		AddToVector(&sort, c);
	}
	return sort;
}

int main(int argc, char *argv[])
{
	String path = "../../Sparky-core/src";
	std::vector<String> files = GetAllFiles(path + "/sp", "*");
	const char* cmdArgs[] = { "-I", path.c_str(), "-c", "-x", "c++", "-stdlib=libstdc++", "-std=c++1y", "-ast-dump" }; 
	CXIndex index = clang_createIndex(1, 0);
	for (const String& file : files)
	{
		if (!EndsWith(file, ".h"))
			continue;

		CXTranslationUnit tu = clang_parseTranslationUnit(index, file.c_str(), cmdArgs, 6, NULL, 0, 0);
		if (tu == NULL)
		{
			printf("Cannot parse translation unit\n");
			return 1;
		}

		CXCursor rootCursor = clang_getTranslationUnitCursor(tu);
		clang_visitChildren(rootCursor, traverse, (CXClientData)NULL);

		clang_disposeTranslationUnit(tu);
	}
	clang_disposeIndex(index);
	GenerateFile(path + "/sp/scripting/API.cpp", SortClasses(classes));
#ifdef SP_DEBUG
	system("PAUSE");
#endif
	return 0;
}