#include "sp.h"

#include "Writer.h"

#include <vector>

#include "utils\String.h"
#include "utils\CTypes.h"

namespace sp { namespace gen {

	void GenerateFile(String filePath, std::vector<Class> classes) {
		for (int i = 0; i < classes.size(); i++) {
			Class c = classes[i];
			String classInfo = c.name;
			for (int j = 0; j < c.methods.size(); j++) {
				Method m = c.methods[j];
				classInfo.append("\n").append("	").append(m.name).append(" ").append(m.type).append(" ");
				switch (m.methodType) {
				case MethodType::CONSTRUCTOR:
					classInfo.append("CONSTRUCTOR");
					break;
				case MethodType::DESTRUCTOR:
					classInfo.append("DESTRUCTOR");
					break;
				case MethodType::METHOD:
					classInfo.append("METHOD");
					break;
				}
				classInfo.append(" (");
				for (int c = 0; c < m.parameters.size(); c++) {
					Parameter p = m.parameters[c];
					if (c != 0) classInfo.append(", ");
					classInfo.append(p.type);
				}
				classInfo.append(")");
			}
			std::cout << classInfo << std::endl;
		}
	}

} }