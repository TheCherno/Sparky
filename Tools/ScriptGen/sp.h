#pragma once

// From core (sp.h)

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>

#include "SPTypes.h"

#define SP_ASSERT(condition) \
	if (!(condition)) __debugbreak();