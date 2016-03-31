#pragma once

#include "sp/sp.h"
#include "sp/Common.h"
#include "sp/Types.h"

#include "sp/maths/maths.h"
#include "sp/utils/FileUtils.h"

#include "ShaderUniform.h"
#include "ShaderResource.h"

namespace sp { namespace graphics { namespace API {

#define SHADER_VERTEX_INDEX		0
#define SHADER_UV_INDEX			1
#define SHADER_MASK_UV_INDEX	2
#define SHADER_TID_INDEX		3
#define SHADER_MID_INDEX		4
#define SHADER_COLOR_INDEX		5

#define SHADER_UNIFORM_PROJECTION_MATRIX_NAME	"sys_ProjectionMatrix"
#define SHADER_UNIFORM_VIEW_MATRIX_NAME			"sys_ViewMatrix"
#define SHADER_UNIFORM_MODEL_MATRIX_NAME		"sys_ModelMatrix"

	class SP_API Shader
	{
	public:
		static const Shader* s_CurrentlyBound;
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetVSSystemUniformBuffer(byte* data, uint size, uint slot = 0) = 0;
		virtual void SetPSSystemUniformBuffer(byte* data, uint size, uint slot = 0) = 0;

		virtual void SetVSUserUniformBuffer(byte* data, uint size) = 0;
		virtual void SetPSUserUniformBuffer(byte* data, uint size) = 0;

		virtual const ShaderUniformBufferList& GetVSSystemUniforms() const = 0;
		virtual const ShaderUniformBufferList& GetPSSystemUniforms() const = 0;
		virtual const ShaderUniformBufferDeclaration* GetVSUserUniformBuffer() const = 0;
		virtual const ShaderUniformBufferDeclaration* GetPSUserUniformBuffer() const = 0;

		virtual const ShaderResourceList& GetResources() const = 0;

		virtual const String& GetName() const = 0;
		virtual const String& GetFilePath() const = 0;

		// virtual void SetData(byte* data, uint size) = 0;

		// bool HasUniform(const String& name) const = 0;
	public:
		static Shader* CreateFromFile(const String& name, const String& filepath, void* address = nullptr); // TODO: Temp, implement properly
		static Shader* CreateFromSource(const String& name, const String& source);
		
		static bool TryCompile(const String& source, String& error);
		static bool TryCompileFromFile(const String& filepath, String& error);
	};

} } }