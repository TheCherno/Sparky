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

			for (int i = 0; i < classes.size(); i++) {
				Class c = classes[i];
				if (c.name == "") continue;
				if (c.name.find("VFS") != std::string::npos) continue;
				if (c.accessType != AccessType::PUBLIC) continue;

				bool consDone = false;
				bool skipCons = false;
				bool noConsPars = false;

				String cons = "SP_CLASSREGISTER";
				cons.append("(");
				cons.append(c.name);

				std::vector<String> methods;
				int currentMethodIndex = 0;
				for (int j = 0; j < c.methods.size(); j++) {
					Method m = c.methods[j];
					if (StartsWith(m.name, "operator")) continue;
					switch (m.methodType) {
					case MethodType::CONSTRUCTOR:
						if (consDone) continue;
						if (m.accessType != AccessType::PUBLIC) continue;
						consDone = true;

						for (int h = 0; h < constructorsDone.size(); h++) {
							String p = constructorsDone[h];
							if (c.name.find(p) != std::string::npos) {
								skipCons = true;
							}
						}
						constructorsDone.push_back(c.name);

						if (m.parameters.size() == 0) {
							noConsPars = true;
						}
						else {
							for (int h = 0; h < m.parameters.size(); h++) {
								Parameter p = m.parameters[h];
								cons.append(", ").append(p.type);
							}
						}
						break;
					case MethodType::DESTRUCTOR:
						break;
					case MethodType::METHOD:
						if (m.accessType == AccessType::PUBLIC) {
							int largest = 0;
							for (int v = 0; v < c.methods.size(); v++) {
								Method other = c.methods[v];
								if (m.name.find(other.name) != std::string::npos) {
									if (other.parameters.size() > largest) largest = other.parameters.size();
								}
							}
							if (largest > m.parameters.size()) continue;

							if (m.isStatic) {

							}
							else
							{
								String parameters;
								for (int h = 0; h < m.parameters.size(); h++) {
									Parameter p = m.parameters[h];
									parameters.append(", ").append(p.type);
								}

								methods.push_back("SP_FUNCTION");
								methods[currentMethodIndex].append("(");
								methods[currentMethodIndex].append(c.name);
								methods[currentMethodIndex].append(", ");
								methods[currentMethodIndex].append(m.name);
								methods[currentMethodIndex].append(", ");
								methods[currentMethodIndex].append(m.type);
								methods[currentMethodIndex].append(parameters);
								methods[currentMethodIndex].append(");");
								currentMethodIndex++;
							}
						}
						break;
					}

				}
				for (int c = 0; c < methods.size(); c++) {
					f << "		" << methods[c].c_str() << std::endl;
				}

				if (!skipCons) {
					if (consDone) {
						if (noConsPars) {
							String defC = "SP_CLASSREGISTERDEF";
							defC.append("(");
							defC.append(c.name);
							defC.append(");");
							f << "		" << defC.c_str() << std::endl;
						}
						else {
							cons.append(");");
							f << "		" << cons.c_str() << std::endl;
						}
					}
					else {
						String defC = "_SP_CLASSREGISTER";
						defC.append("(");
						defC.append(c.name);
						defC.append(");");
						f << "		" << defC.c_str() << std::endl;
					}
				}
			}

			f << "	}\n";
			f << "} }\n";
		}
	}

} }