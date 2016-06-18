#pragma once

#include "utils\CTypes.h"
#include "utils\String.h"

namespace sp { namespace gen {

	void GenerateFile(String filePath, std::map<String, Class> classes);

} }