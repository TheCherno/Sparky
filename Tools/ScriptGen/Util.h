#pragma once

#include <stdlib.h>
#include <vector>

#include "String.h"
#include "SPTypes.h"

static char* ReadFile(const String& filepath)
{
	FILE* file = fopen(filepath.c_str(), "r");

	fseek(file, 0, SEEK_END);
	unsigned long length = ftell(file);
	char* data = new char[length + 1];
	memset(data, 0, length + 1);
	fseek(file, 0, SEEK_SET);

	fread(data, 1, length, file);
	fclose(file);

	return data;
}