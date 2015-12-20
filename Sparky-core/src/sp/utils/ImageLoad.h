#pragma once

#include <string>

#include <sp/Types.h>
#include <sp/utils/Log.h>

namespace sp {

	SP_API byte* load_image(const char* filename, uint* width, uint* height, uint* bits);

}