#pragma once

// From core (sp.h)

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>

typedef unsigned int uint;
typedef unsigned char byte;

#define SP_ASSERT(condition) \
	if (!(condition)) __debugbreak();