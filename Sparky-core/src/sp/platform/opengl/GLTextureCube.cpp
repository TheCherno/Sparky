#include "sp/sp.h"
#include "GLTextureCube.h"

#include <GL/glew.h>
#include <FreeImage.h>

#include "sp/utils/ImageLoad.h"

#include "sp/system/Memory.h"

#include "GLTexture2D.h"

namespace sp { namespace graphics { namespace API {

	GLTextureCube::GLTextureCube(const String& name, const String& filepath)
	{
		m_Name = name;
		m_Files[0] = filepath;
		m_Handle = LoadFromSingleFile();
	}

	GLTextureCube::GLTextureCube(const String& name, const String* files)
	{
		m_Name = name;
		m_Files = spnew String[6];
		for (uint i = 0; i < 6; i++)
			m_Files[i] = files[i];
		m_Handle = LoadFromMultipleFiles();
	}

	GLTextureCube::GLTextureCube(const String& name, const String* files, uint mips, InputFormat format)
	{
		m_Name = name;
		m_Files = spnew String[mips];
		for (uint i = 0; i < mips; i++)
			m_Files[i] = files[i];
		if (format == InputFormat::VERTICAL_CROSS)
			m_Handle = LoadFromVCross(mips);
	}

	GLTextureCube::~GLTextureCube()
	{
		GLCall(glDeleteTextures(1, &m_Handle));
	}

	void GLTextureCube::Bind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle));
	}

	void GLTextureCube::Unbind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}

	uint GLTextureCube::LoadFromSingleFile()
	{
		// TODO: Implement
		return 0;
	}

	uint GLTextureCube::LoadFromMultipleFiles()
	{
		const String& xpos = m_Files[0];
		const String& xneg = m_Files[1];
		const String& ypos = m_Files[2];
		const String& yneg = m_Files[3];
		const String& zpos = m_Files[4];
		const String& zneg = m_Files[5];

		m_Parameters.format = TextureFormat::RGBA;

		uint width, height, bits;
		byte* xp = LoadImage(xpos, &width, &height, &bits, true);
		byte* xn = LoadImage(xneg, &width, &height, &bits, true);
		byte* yp = LoadImage(ypos, &width, &height, &bits, true);
		byte* yn = LoadImage(yneg, &width, &height, &bits, true);
		byte* zp = LoadImage(zpos, &width, &height, &bits, true);
		byte* zn = LoadImage(zneg, &width, &height, &bits, true);

		uint result;
		GLCall(glGenTextures(1, &result));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, result));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		uint internalFormat = GLTexture2D::SPTextureFormatToGL(m_Parameters.format);
		uint format = internalFormat;

		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, xp));
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, xn));

		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, yp));
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, yn));

		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, zp));
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, zn));

		GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

		return result;
	}

	uint GLTextureCube::LoadFromVCross(uint mips)
	{
		uint srcWidth, srcHeight, bits;
		byte*** cubeTextureData = spnew byte**[mips];
		for (uint i = 0; i < mips; i++)
			cubeTextureData[i] = spnew byte*[6];

		uint* faceWidths = spnew uint[mips];
		uint* faceHeights = spnew uint[mips];

		m_Parameters.format = TextureFormat::RGBA;
		for (uint m = 0; m < mips; m++)
		{
			byte* data = LoadImage(m_Files[m], &srcWidth, &srcHeight, &bits, !m_LoadOptions.flipY);
			uint stride = bits / 8;

			uint face = 0;
			uint faceWidth = srcWidth / 3;
			uint faceHeight = srcHeight / 4;
			faceWidths[m] = faceWidth;
			faceHeights[m] = faceHeight;
			for (uint cy = 0; cy < 4; cy++)
			{
				for (uint cx = 0; cx < 3; cx++)
				{
					if (cy == 0 || cy == 2 || cy == 3)
					{
						if (cx != 1)
							continue;
					}

					cubeTextureData[m][face] = spnew byte[faceWidth * faceHeight * stride];
					uint index = 0;
					for (uint y = 0; y < faceHeight; y++)
					{
						uint offset = y;
						if (face == 5)
							offset = faceHeight - (y + 1);
						uint yp = cy * faceHeight + offset;
						for (uint x = 0; x < faceWidth; x++)
						{
							offset = x;
							if (face == 5)
								offset = faceWidth - (x + 1);
							uint xp = cx * faceWidth + offset;
							cubeTextureData[m][face][(x + y * faceWidth) * stride + 0] = data[(xp + yp * srcWidth) * stride + 0];
							cubeTextureData[m][face][(x + y * faceWidth) * stride + 1] = data[(xp + yp * srcWidth) * stride + 1];
							cubeTextureData[m][face][(x + y * faceWidth) * stride + 2] = data[(xp + yp * srcWidth) * stride + 2];
							if (stride >= 4)
								cubeTextureData[m][face][(x + y * faceWidth) * stride + 3] = data[(xp + yp * srcWidth) * stride + 3];
						}
					}
					face++;
				}
			}
			spdel[] data;
		}

		uint result;
		GLCall(glGenTextures(1, &result));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, result));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		// API::SetTextureParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, mips - 1);
		// API::SetTextureParameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, mips - 1);

		uint internalFormat = GLTexture2D::SPTextureFormatToGL(m_Parameters.format);
		uint format = internalFormat;
		for (uint m = 0; m < mips; m++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, m, internalFormat, faceWidths[m], faceHeights[m], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[m][3]));
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m, internalFormat, faceWidths[m], faceHeights[m], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[m][1]));

			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m, internalFormat, faceWidths[m], faceHeights[m], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[m][0]));
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m, internalFormat, faceWidths[m], faceHeights[m], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[m][4]));

			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m, internalFormat, faceWidths[m], faceHeights[m], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[m][2]));
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m, internalFormat, faceWidths[m], faceHeights[m], 0, format, GL_UNSIGNED_BYTE, cubeTextureData[m][5]));
		}
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

		for (uint m = 0; m < mips; m++)
		{
			for (uint f = 0; f < 6; f++)
			{
				spdel[] cubeTextureData[m][f];
			}
			spdel[] cubeTextureData[m];
		}
		spdel[] cubeTextureData;

		return result;
	}

} } }