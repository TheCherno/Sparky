#include "sp.h"

#include <iostream>
#include <vector>

#include <clang-c/Index.h>

#include "utils/CTypes.h"
#include "utils/String.h"

#include "Writer.h"

#include <stack>
#include <Windows.h>

using namespace sp::gen;

std::vector<Class> classes;
std::vector<String> namespaceStack;

Class* currentClass = nullptr;
Method* currentMethod = nullptr;

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
		classes.push_back(Class(name, accessType));
		currentClass = &classes.back();
		currentClass->namespaceStack = namespaceStack;
		namespaceStack.clear();
	}
	else if (type == "StructDecl")
	{
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
		classes.push_back(Class(name, accessType));
		currentClass = &classes.back();
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
		currentClass->baseClass = SplitString(name, " ").back();
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

void Sort(std::vector<Class> &classes)
{
	std::vector<bool> tbr(classes.size());

	for (int x = 0; x < tbr.size(); ++x)
		tbr[x] = false;

	for (int i = 0; i < classes.size(); i++) 
	{
		for (int j = 0; j < classes.size(); j++) 
		{
			if (tbr[j]) continue;
			if (j == i) continue;
			if (j <= i) continue;
			if (classes[i].name.find(classes[j].name) == std::string::npos)
			{
				if (classes[i].methods.size() < classes[j].methods.size())
				{
					tbr[j] = true;
					std::cout << j << ", " << i << std::endl;
				}
			}
		}
	}


	for (int i = tbr.size() - 1; i >= 0; i--)
	{
		if (tbr[i])
		{
			classes.erase(classes.begin() + i);
		}
	}
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
	// Sort(classes);
	clang_disposeIndex(index);
	GenerateFile(path + "/sp/scripting/API.h", classes);
#ifdef SP_DEBUG
	system("PAUSE");
#endif
	return 0;
}