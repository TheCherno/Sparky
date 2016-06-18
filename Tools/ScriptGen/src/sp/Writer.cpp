#include "sp.h"

#include "Writer.h"

#include <vector>
#include <iostream>
#include <fstream>

#include "utils\String.h"
#include "utils\CTypes.h"

namespace sp { namespace gen {

	bool ParseParameters(std::vector<Parameter> parameters, String& out) {
		for (int h = 0; h < parameters.size(); h++) {
			Parameter p = parameters[h];
			if (p.name.find("T") != std::string::npos) return true;
			if (p.name.find("RenderTarget") != std::string::npos) return true;
			if (h != 0) out.append(", ");
			out.append(p.type);
		}
		return false;
	}

	void GenerateFile(String filePath, std::map<String, Class> classes) {
		std::ofstream f(filePath);

		String namespaces[] = { "sp", "graphics", "audio", "debug", "entity", "component", "internal", "API", "ui", "events", "maths", "ftgl"};

		std::vector<String> constructorsDone;

		if (f.is_open()) {

			String pre;

			pre.append("#pragma once\n");
			pre.append("\n");
			pre.append("#ifndef LUA_H\n");
			pre.append("#define LUA_H\n");
			pre.append("#include <lua.hpp>\n");
			pre.append("#include \"luawrapper.h\"\n");
			pre.append("#include \"luawrapperutils.h\"\n");
			pre.append("#endif\n");
			pre.append("\n");
			pre.append("#include \"Sparky.h\"\n");
			pre.append("\n");

			for (int i = 0; i < sizeof(namespaces) / sizeof(String); i++) {
				pre.append("using namespace ");
				pre.append(namespaces[i]);
				pre.append(";\n");
			}

			pre.append("\n");

			pre.append("namespace sp { namespace scripting {\n");
			pre.append("	void Load(lua_State* L)	{\n");

			f << pre.c_str();

			f << "		luabind::module(L)\n		[" << std::endl;

			bool isStart = true;
			for (std::map<String, Class>::iterator iter = classes.begin(); iter != classes.end(); ++iter)
			{
				Class c = iter->second;
				if (c.name == "") continue;
				if (c.name.find("VFS") != std::string::npos) continue;
				if (c.name.find("Event") != std::string::npos) continue;
				if (c.name.find("Panel") != std::string::npos) continue;
				if (c.name.find("Button") != std::string::npos) continue;
				if (c.name.find("Widget") != std::string::npos) continue;
				if (c.name.find("MaterialInstance") != std::string::npos) continue;
				if (c.accessType != AccessType::PUBLIC) continue;


				if (!isStart) {
					f << ",\n\n";
				}
				else {
					isStart = false;
				}

				bool constructorDone = false, canSkipConstructor = false;

				String constructorParameters;

				std::vector<String> methods;
				int currentMethodIndex = 0;
				for (int j = 0; j < c.methods.size(); j++) {
					Method m = c.methods[j];
					if (StartsWith(m.name, "operator")) continue;

					switch (m.methodType) {
					case MethodType::CONSTRUCTOR:
						if (constructorDone) continue;
						if (canSkipConstructor) continue;
						if (m.accessType != AccessType::PUBLIC) continue;
						constructorDone = true;

						/*
						for (int h = 0; h < constructorsDone.size(); h++) {
							String p = constructorsDone[h];
							if (c.name.find(p) != std::string::npos) {
								canSkipConstructor = true;
							}
						}
						*/

						constructorsDone.push_back(c.name);
						ParseParameters(m.parameters, constructorParameters);
						break;
					case MethodType::DESTRUCTOR:
						break;
					case MethodType::METHOD:
						if (m.accessType == AccessType::PUBLIC) {
							String parameters;
							if (ParseParameters(m.parameters, parameters)) continue;
							if (m.isStatic) {

							}
							else
							{
								

								methods.push_back("def");
								methods[currentMethodIndex].append("(\"");
								methods[currentMethodIndex].append(m.name);
								methods[currentMethodIndex].append("\", (");
								methods[currentMethodIndex].append(m.type);
								methods[currentMethodIndex].append("(");
								methods[currentMethodIndex].append(c.name);
								methods[currentMethodIndex].append("::*)(");
								methods[currentMethodIndex].append(parameters);
								methods[currentMethodIndex].append("))&");
								methods[currentMethodIndex].append(c.name);
								methods[currentMethodIndex].append("::");
								methods[currentMethodIndex].append(m.name);
								methods[currentMethodIndex].append(")");
								currentMethodIndex++;
							}
						}
						break;
					}

				}

				if (c.baseClass == "" || c.baseClass.empty()) {
					f << "			luabind::class_<" + c.name + ">(\"" + c.name + "\")";
				}
				else {
					f << "			luabind::class_<" + c.name + ", " + c.baseClass + ">(\"" + c.name + "\")";
				}

				if (constructorDone) {
					String defC = "def(luabind::constructor<";
					defC.append(constructorParameters);
					defC.append(">())");
					f << std::endl << "				." << defC.c_str();
				}

				for (int c = 0; c < methods.size(); c++) {
					f << std::endl << "				." << methods[c].c_str();
				}
			}

			f << std::endl << "		];" << std::endl;

			f << "	}" << std::endl << "} }";
		}
	}

} }