#include "CubeMap.h"

namespace sparky { namespace graphics {

	CubeMap::CubeMap(const std::string& path, unsigned int width, unsigned int height)
		: m_Path(path), m_Width(width), m_Height(height)
	{
		load();
	}

	void CubeMap::load()
	{
		const int TOP		= 0;
		const int BOTTOM	= 1;
		const int LEFT		= 2;
		const int RIGHT		= 3;
		const int FRONT		= 4;
		const int BACK		= 5;

		GLsizei iw, ih;
		unsigned int* pixels = load_image_pixels(m_Path.c_str(), &iw, &ih);

		unsigned int** face = new unsigned int*[6];

		// Top
		{
			face[TOP] = new unsigned int[m_Width * m_Height];
			int xo = 2 * m_Width;
			int yo = 0 * m_Height;
			for (int y = 0; y < m_Height; y++)
			{
				for (int x = 0; x < m_Width; x++)
				{
					face[TOP][x + y * m_Width] = pixels[(x + xo) + (y + yo) * iw];
				}
			}
		}
		// Bottom
		{
			face[BOTTOM] = new unsigned int[m_Width * m_Height];
			int xo = 2 * m_Width;
			int yo = 2 * m_Height;
			for (int y = 0; y < m_Height; y++)
			{
				for (int x = 0; x < m_Width; x++)
				{
					face[BOTTOM][x + y * m_Width] = pixels[(x + xo) + (y + yo) * iw];
				}
			}
		}
		// Left
		{
			face[LEFT] = new unsigned int[m_Width * m_Height];
			int xo = 1 * m_Width;
			int yo = 1 * m_Height;
			for (int y = 0; y < m_Height; y++)
			{
				for (int x = 0; x < m_Width; x++)
				{
					face[LEFT][x + y * m_Width] = pixels[(x + xo) + (y + yo) * iw];
				}
			}
		}
		// Right
		{
			face[RIGHT] = new unsigned int[m_Width * m_Height];
			int xo = 3 * m_Width;
			int yo = 1 * m_Height;
			for (int y = 0; y < m_Height; y++)
			{
				for (int x = 0; x < m_Width; x++)
				{
					face[RIGHT][x + y * m_Width] = pixels[(x + xo) + (y + yo) * iw];
				}
			}
		}
		// Front
		{
			face[FRONT] = new unsigned int[m_Width * m_Height];
			int xo = 2 * m_Width;
			int yo = 1 * m_Height;
			for (int y = 0; y < m_Height; y++)
			{
				for (int x = 0; x < m_Width; x++)
				{
					face[FRONT][x + y * m_Width] = pixels[(x + xo) + (y + yo) * iw];
				}
			}
		}
		// Back
		{
			face[BACK] = new unsigned int[m_Width * m_Height];
			int xo = 0 * m_Width;
			int yo = 1 * m_Height;
			for (int y = 0; y < m_Height; y++)
			{
				for (int x = 0; x < m_Width; x++)
				{
					face[BACK][x + y * m_Width] = pixels[(x + xo) + (y + yo) * iw];
				}
			}
		}

		glGenTextures(1, &m_TID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TID);

		for (int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, face[i]);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		delete[] pixels;
	}

	void CubeMap::bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TID);
	}

	void CubeMap::unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

} }