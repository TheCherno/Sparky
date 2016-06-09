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
	String type = String(clang_getCString(cursorKind)); // TODO: Use enums instead of strings
	String name = clang_getCString(cursorSpelling);

 	if (!clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
 		return CXChildVisit_Recurse;

	if (type == "Namespace")
	{
		namespaceStack.push_back(name);
	}
	else if (type == "ClassDecl")
	{
		classes.push_back(Class(name));
		currentClass = &classes.back();
		currentClass->namespaceStack = namespaceStack;
		namespaceStack.clear();
	}
	else if (type == "CXXConstructor")
	{
		if (currentClass)
		{
			SP_ASSERT(currentClass);
			currentClass->methods.push_back(Method(MethodType::CONSTRUCTOR));
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
			currentClass->methods.push_back(Method(name, String(clang_getCString(resultType)), isStatic));
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

int main(int argc, char *argv[])
{
	String path = "../../Sparky-core/src/sp/debug/";
	std::vector<String> files = GetAllFiles("../../Sparky-core/src/sp", "*");

	String includePath = "../../Sparky-core/src";
	const char* cmdArgs[] = { "-I", includePath.c_str(), "-c", "-x", "c++", "-std=c++14", "-ast-dump" }; // Note: -c is ignored according to API docs

	// create index w/ excludeDeclsFromPCH = 1, displayDiagnostics=1.
	CXIndex index = clang_createIndex(1, 0);
	for (const String& file : files)
	{
		if (!EndsWith(file, ".h"))
			continue;

		std::cout << "Reading file: " << file << std::endl;

		// create Translation Unit
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
	GenerateFile("test.txt", classes);
	system("PAUSE");
	return 0;
}