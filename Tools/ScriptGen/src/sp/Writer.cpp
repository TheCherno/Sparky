#include "sp.h"

#include "Writer.h"

#include <vector>
#include <iostream>
#include <fstream>

#include "utils\String.h"
#include "utils\CTypes.h"

namespace sp { namespace gen {

	bool ParseParameters(std::vector<Parameter> parameters, String& out) {
		bool found = false;
		for (int h = 0; h < parameters.size(); h++) {
			Parameter p = parameters[h];
			if (p.type.find(" T ") != std::string::npos ||
				p.type.find("RenderTarget") != std::string::npos)
				{
					found = true;
					break;
				}
			if (h != 0) out.append(", ");
			out.append(p.type);
		}
#ifdef SP_DEBUG
		if (found) std::cout << found << std::endl;
#endif
		return found;
	}

	void GenerateFile(String filePath, std::vector<Class> classes) {
		std::ofstream f(filePath);

		String namespaces[] = { "sp", "graphics", "audio", "debug", "entity", "component", "internal", "API", "ui", "events", "maths", "ftgl"};
		String ignoreMethods[] = { 
			".def(\"Multiply\", (sp::maths::vec4(vec4::*)(const sp::maths::mat4 &))&vec4::Multiply)",
			".def(\"Multiply\", (sp::maths::vec3(vec3::*)(const sp::maths::mat4 &))&vec3::Multiply)"
		};

		std::vector<String> constructorsDone;

		if (f.is_open()) {

			String pre;

			pre.append("#include \"sp\\sp.h\"\n");
			pre.append("#include \"API.h\"\n");
			pre.append("#include <lua.hpp>\n");
			pre.append("#include <luabind\\luabind.hpp>\n");
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
			for (int i = 0; i < classes.size(); i++)
			{
				Class c = classes[i];
				if (c.name == "") continue;
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

								if (m.type.find("T ") != std::string::npos ||
									m.type.find("RenderTarget") != std::string::npos)
									continue;

								if (c.name.find("vec3") != std::string::npos &&
									m.name.find("Multiply") != std::string::npos)
									continue;

								if (c.name.find("vec4") != std::string::npos &&
									m.name.find("Multiply") != std::string::npos)
									continue;

								if (c.name.find("BooleanAction") != std::string::npos &&
									m.name.find("Getter") != std::string::npos)
									continue;

								if (c.name.find("BufferLayout") != std::string::npos &&
									m.name.find("Push") != std::string::npos)
									continue;

								if (c.name.find("AABB") != std::string::npos &&
									m.name.find("Contains") != std::string::npos)
									continue; 

								if (c.name.find("Font") != std::string::npos &&
									m.name.find("GetFTFont") != std::string::npos)
									continue;

								String method = "def";
								method.append("(\"");
								method.append(m.name);
								method.append("\", (");
								method.append(m.type);
								method.append("(");
								method.append(c.name);
								method.append("::*)(");
								method.append(parameters);
								method.append("))&");
								method.append(c.name);
								method.append("::");
								method.append(m.name);
								method.append(")");

								bool ignore = false;
								for (int b = 0; b < sizeof(ignoreMethods) / sizeof(String); b++) {
									if (IgnoreCaseEquals(method, ignoreMethods[b])) {
										ignore = true;
										break;
									}
								}
								if (!ignore) {
									methods.push_back(method);
									currentMethodIndex++;
								}
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