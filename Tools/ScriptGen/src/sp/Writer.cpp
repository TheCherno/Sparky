#include "sp.h"

#include "Writer.h"

#include <vector>
#include <iostream>
#include <fstream>

#include "utils\String.h"
#include "utils\CTypes.h"

namespace sp { namespace gen {

	void GenerateFile(String filePath, std::vector<Class> classes) {
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

			for (int i = 0; i < classes.size(); i++) {
				Class c = classes[i];
				if (c.name == "") continue;
				// if (c.name.find("VFS") != std::string::npos) continue;
				if (c.accessType != AccessType::PUBLIC) continue;


				if (i != 0) f << ",\n\n";

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

						for (int h = 0; h < m.parameters.size(); h++) {
							Parameter p = m.parameters[h];
							if (h != 0) constructorParameters.append(", ");
							constructorParameters.append(p.type);
						}
						break;
					case MethodType::DESTRUCTOR:
						break;
					case MethodType::METHOD:
						if (m.accessType == AccessType::PUBLIC) {

							if (m.isStatic) {

							}
							else
							{
								String parameters;
								for (int h = 0; h < m.parameters.size(); h++) {
									Parameter p = m.parameters[h];
									if (h != 0) parameters.append(", ");
									parameters.append(p.type);
								}

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


				f << "			luabind::class_<" + c.name + ">(\"" + c.name + "\")";

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