#pragma once

#include "utils\CTypes.h"
#include "utils\String.h"

namespace sp { namespace gen {

	void GenerateFile(String filePath, std::vector<Class> classes);

} }