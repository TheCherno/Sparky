#include "sp/sp.h"
#include "GLTexture2D.h"

#include <GL/glew.h>
#include <FreeImage.h>

#include "sp/utils/ImageLoad.h"
#include "sp/platform/opengl/GLShader.h"
#include "sp/system/Memory.h"

namespace sp { namespace graphics { namespace API {

	GLTexture2D::GLTexture2D(uint width, uint height, TextureParameters parameters, bool renderTarget)
		: m_FileName("NULL")
	{
		m_Width = width;
		m_Height = height;
		m_Parameters = parameters;
		m_Handle = Load();
	}

	GLTexture2D::GLTexture2D(uint width, uint height, uint color, TextureParameters parameters, TextureLoadOptions loadOptions)
		: m_FileName("NULL")
	{
		m_Width = width;
		m_Height = height;
		m_Parameters = parameters;
		m_LoadOptions = loadOptions;
		m_Handle = Load();

		SetData(color);
	}

	GLTexture2D::GLTexture2D(const String& name, const String& filename, TextureParameters parameters, TextureLoadOptions loadOptions)
		: m_FileName(filename)
	{
		m_Name = name;
		m_Parameters = parameters;
		m_LoadOptions = loadOptions;
		m_Handle = Load();
	}

	GLTexture2D::~GLTexture2D()
	{
		GLCall(glDeleteTextures(1, &m_Handle));
	}

	uint GLTexture2D::Load()
	{
		// TODO: Split this up into loading from file vs. generating from data
		byte* pixels = nullptr;
		if (m_FileName != "NULL")
		{
			uint bits;
			pixels = LoadImage(m_FileName.c_str(), &m_Width, &m_Height, &bits, !m_LoadOptions.flipY); // FreeImage loads bottom->top
			if (bits != 24 && bits != 32)
				SP_ERROR("[Texture] Unsupported image bit-depth! (", bits, ")");
			m_Parameters.format = bits == 24 ? TextureFormat::RGB : TextureFormat::RGBA;
		}
		
		uint handle;
		GLCall(glGenTextures(1, &handle));
		GLCall(glBindTexture(GL_TEXTURE_2D, handle));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Parameters.filter == TextureFilter::LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Parameters.filter == TextureFilter::LINEAR ? GL_LINEAR : GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, SPTextureWrapToGL(s_WrapMode)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, SPTextureWrapToGL(s_WrapMode)));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, SPTextureFormatToGL(m_Parameters.format), m_Width, m_Height, 0, SPTextureFormatToGL(m_Parameters.format), GL_UNSIGNED_BYTE, pixels ? pixels : NULL));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (pixels != nullptr)
			spdel[] pixels;

		return handle;
	}

	void GLTexture2D::SetData(const void* pixels)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Handle));
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, SPTextureFormatToGL(m_Parameters.format), GL_UNSIGNED_BYTE, pixels));
	}

	void GLTexture2D::SetData(uint color)
	{
		SP_ASSERT(false, "Broken");
		uint stride = m_Parameters.format == TextureFormat::RGB ? 3 : 4;
		uint size = m_Width * m_Height * stride;
		byte* data = new byte[size];
		for (uint i = 0; i < size; i += stride)
		{
			data[i + 0] = (color & 0xff);
			data[i + 1] = (color & 0xff00) >> 8;
			data[i + 2] = (color & 0xff0000) >> 16;
			if (stride == 4)
				data[i + 3] = (color & 0xff000000) >> 24;
		}

		SetData(data);
		spdel[] data;
	}

	void GLTexture2D::Bind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Handle));
	}

	void GLTexture2D::Unbind(uint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	uint GLTexture2D::SPTextureFormatToGL(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RGBA:				return GL_RGBA;
			case TextureFormat::RGB:				return GL_RGB;
			case TextureFormat::LUMINANCE:			return GL_LUMINANCE;
			case TextureFormat::LUMINANCE_ALPHA:	return GL_LUMINANCE_ALPHA;
		}
		return 0;
	}

	uint GLTexture2D::SPTextureWrapToGL(TextureWrap wrap)
	{
		switch (wrap)
		{
			case TextureWrap::CLAMP:			return GL_CLAMP;
			case TextureWrap::CLAMP_TO_BORDER:	return GL_CLAMP_TO_BORDER;
			case TextureWrap::CLAMP_TO_EDGE:	return GL_CLAMP_TO_EDGE;
			case TextureWrap::REPEAT:			return GL_REPEAT;
			case TextureWrap::MIRRORED_REPEAT:	return GL_MIRRORED_REPEAT;
		}
		return 0;
	}

} } }