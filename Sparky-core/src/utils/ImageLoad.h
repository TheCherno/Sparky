#pragma once

#include <string>
#include <FreeImage.h>
#include <FreeImage/Utilities.h>

namespace sparky {

	static BYTE* load_image(const char* filename, GLsizei* width, GLsizei* height)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib = nullptr;
		fif = FreeImage_GetFileType(filename, 0);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		if (fif == FIF_UNKNOWN)
			return nullptr;

		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		if (!dib)
			return nullptr;

		BYTE* pixels = FreeImage_GetBits(dib);
		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
		int bits = FreeImage_GetBPP(dib);
		
#ifdef SPARKY_EMSCRIPTEN
		SwapRedBlue32(dib);
#endif

		int size = *width * *height * (bits / 8);
		BYTE* result = new BYTE[size];
		memcpy(result, pixels, size);
		FreeImage_Unload(dib);
		return result;
	}

	static unsigned int* load_image_pixels(const char* filename, GLsizei* width, GLsizei* height)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib = nullptr;
		fif = FreeImage_GetFileType(filename, 0);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		if (fif == FIF_UNKNOWN)
			return nullptr;

		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		if (!dib)
			return nullptr;

		BYTE* data = FreeImage_GetBits(dib);
		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
		int bits = FreeImage_GetBPP(dib);

		unsigned int* result = new unsigned int[*width * *height];
		for (int i = 0; i < *width * *height; i++)
		{
			int offset = i * 3;
			result[i] = data[offset + 2] << 24 | data[offset + 1] << 16 | data[offset] << 8 | 0xff;
		}

		FreeImage_Unload(dib);
		return result;
	}

}