#include "sp/sp.h"
#include "FileUtils.h"

namespace sp { namespace utils {

	String ReadFile(const String& filepath)
	{
		FILE* file = fopen(filepath.c_str(), "rb");
		if (file == nullptr)
			SP_ASSERT(file, "Could not open file '", filepath, "'!");

		fseek(file, 0, SEEK_END);
		int32 length = ftell(file);
		SP_ASSERT(length < 100 * 1024 * 1024);
		String result(length, 0);
		fseek(file, 0, SEEK_SET);
		fread(&result[0], 1, length, file);
		fclose(file);

		// Strip carriage returns
		result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
		return result;
	}

} }