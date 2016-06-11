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

		String namespaces[] = { "sp", "graphics", "audio", "debug", "entity", "component", "internal", "API", "ui", "events", "maths"};

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

				bool consDone = false;

				String cons = "SP_CLASSREGISTER";
				cons.append("(");
				cons.append(c.name);

				if (c.name.find("VFS") != std::string::npos) continue;

				String classData;
				for (int j = 0; j < c.methods.size(); j++) {
					Method m = c.methods[j];
					switch (m.methodType) {
					case MethodType::CONSTRUCTOR:
						if (consDone) continue;
						if (m.accessType != AccessType::PUBLIC) continue;
						consDone = true;
						if (m.parameters.size() == 0) {
							cons.insert(0, "_");
						} else for (int c = 0; c < m.parameters.size(); c++) {
							Parameter p = m.parameters[c];
							cons.append(", ").append(p.type);
						}
						break;
					case MethodType::DESTRUCTOR:
						break;
					case MethodType::METHOD:
						for (int c = 0; c < m.parameters.size(); c++) {
							Parameter p = m.parameters[c];
							if (c != 0) classData.append(", ");
							classData.append(p.type);
						}
						break;
					}

				}
				if (consDone) {
					cons.append(");\n");
					f << "		" << cons.c_str();
				}
			}

			f << "}\n";
			f << "}\n";
			f << "}\n";
		}
	}

} }