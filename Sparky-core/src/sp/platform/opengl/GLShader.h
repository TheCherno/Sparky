#pragma once

#include "sp/graphics/shaders/Shader.h"
#include "GLCommon.h"
#include "GLShaderUniform.h"
#include "GLShaderResource.h"

namespace sp { namespace graphics { namespace API {

	struct GLShaderErrorInfo
	{
		uint shader;
		String message[2];
		uint line[2];
	};

	class GLShader : public Shader
	{
	private:
		friend class Shader;
		friend class ShaderManager;
	private:
		uint m_Handle;
		String m_Name, m_Path;
		String m_Source;
		String m_VertexSource, m_FragmentSource;

		ShaderUniformBufferList m_VSUniformBuffers;
		ShaderUniformBufferList m_PSUniformBuffers;
		GLShaderUniformBufferDeclaration* m_VSUserUniformBuffer;
		GLShaderUniformBufferDeclaration* m_PSUserUniformBuffer;
		ShaderResourceList m_Resources;
		ShaderStructList m_Structs;
	public:
		GLShader(const String& name, const String& source);
		~GLShader();

		void Init();
		void Shutdown();
		void Bind() const override;
		void Unbind() const override;

		void SetVSSystemUniformBuffer(byte* data, uint size, uint slot) override;
		void SetPSSystemUniformBuffer(byte* data, uint size, uint slot) override;

		void SetVSUserUniformBuffer(byte* data, uint size) override;
		void SetPSUserUniformBuffer(byte* data, uint size) override;

		void SetUniform(const String& name, byte* data);
		void ResolveAndSetUniformField(const GLShaderUniformDeclaration& field, byte* data, int32 offset);

		inline const String& GetName() const override { return m_Name; }
		inline const String& GetFilePath() const override { return m_Path; }

		inline const ShaderUniformBufferList& GetVSSystemUniforms() const override { return m_VSUniformBuffers; }
		inline const ShaderUniformBufferList& GetPSSystemUniforms() const override { return m_PSUniformBuffers; }
		inline const ShaderUniformBufferDeclaration* GetVSUserUniformBuffer() const override { return m_VSUserUniformBuffer; }
		inline const ShaderUniformBufferDeclaration* GetPSUserUniformBuffer() const override { return m_PSUserUniformBuffer; }
		inline const ShaderResourceList& GetResources() const override { return m_Resources; }
	private:
		static uint Compile(String** shaders, GLShaderErrorInfo& info = GLShaderErrorInfo());
		static void PreProcess(const String& source, String** shaders);

		void Parse(const String& vertexSource, const String& fragmentSource);
		void ParseUniform(const String& statement, uint shaderType);
		void ParseUniformStruct(const String& block, uint shaderType);

		bool IsTypeStringResource(const String& type);

		ShaderStruct* FindStruct(const String& name);

		void ResolveUniforms();
		void ValidateUniforms();
		bool IsSystemUniform(ShaderUniformDeclaration* uniform) const;
		int32 GetUniformLocation(const String& name);

		ShaderUniformDeclaration* FindUniformDeclaration(const String& name, const ShaderUniformBufferDeclaration* buffer);
		ShaderUniformDeclaration* FindUniformDeclaration(const String& name);

		void ResolveAndSetUniforms(ShaderUniformBufferDeclaration* buffer, byte* data, uint size);
		void ResolveAndSetUniform(GLShaderUniformDeclaration* uniform, byte* data, uint size);

		void SetUniformStruct(GLShaderUniformDeclaration* uniform, byte* data, int32 offset);

		void SetUniform1f(const String& name, float value);
		void SetUniform1fv(const String& name, float* value, int32 count);
		void SetUniform1i(const String& name, int32 value);
		void SetUniform1iv(const String& name, int32* value, int32 count);
		void SetUniform2f(const String& name, const maths::vec2& vector);
		void SetUniform3f(const String& name, const maths::vec3& vector);
		void SetUniform4f(const String& name, const maths::vec4& vector);
		void SetUniformMat4(const String& name, const maths::mat4& matrix);

		void SetUniform1f(uint location, float value);
		void SetUniform1fv(uint location, float* value, int32 count);
		void SetUniform1i(uint location, int32 value);
		void SetUniform1iv(uint location, int32* value, int32 count);
		void SetUniform2f(uint location, const maths::vec2& vector);
		void SetUniform3f(uint location, const maths::vec3& vector);
		void SetUniform4f(uint location, const maths::vec4& vector);
		void SetUniformMat4(uint location, const maths::mat4& matrix);
	public:
		static bool TryCompile(const String& source, String& error);
		static bool TryCompileFromFile(const String& filepath, String& error);
	};

} } }