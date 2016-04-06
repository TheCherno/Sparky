#pragma once

#include "sp/String.h"
#include "sp/graphics/shaders/Shader.h"
#include "DXCommon.h"

#include "DXShaderResource.h"
#include "DXShaderUniform.h"

namespace sp { namespace graphics { namespace API {

	struct D3DShaderErrorInfo
	{
		String profile;
		String message;
	};

	class D3DShader : public Shader
	{
	private:
		friend class Shader;
		friend class ShaderManager;
	private:
		struct Data
		{
			ID3D11VertexShader* vertexShader;
			ID3D11PixelShader* pixelShader;
			ID3DBlob* vs;
			ID3DBlob* ps;
		};
	private:
		static const D3DShader* s_CurrentlyBound;
	private:
		String m_Name;
		String m_FilePath;
		mutable Data m_Data;

		ShaderUniformBufferList m_VSUniformBuffers;
		ShaderUniformBufferList m_PSUniformBuffers;
		D3DShaderUniformBufferDeclaration* m_VSUserUniformBuffer;
		D3DShaderUniformBufferDeclaration* m_PSUserUniformBuffer;
		ShaderResourceList m_Resources;
		ShaderStructList m_Structs;

		ID3D11Buffer** m_VSConstantBuffers;
		uint m_VSConstantBuffersCount;

		ID3D11Buffer** m_PSConstantBuffers;
		uint m_PSConstantBuffersCount;
	public:
		D3DShader(const String& name, const String& source);
		~D3DShader();

		inline Data& GetData() const { return m_Data; }

		void Bind() const override;
		void Unbind() const override;

		void SetVSSystemUniformBuffer(byte* data, uint size, uint slot = 0) override;
		void SetPSSystemUniformBuffer(byte* data, uint size, uint slot = 0) override;

		void SetVSUserUniformBuffer(byte* data, uint size) override;
		void SetPSUserUniformBuffer(byte* data, uint size) override;

		inline const String& GetName() const override { return m_Name; }
		inline const String& GetFilePath() const override { return m_FilePath; }

		inline const ShaderUniformBufferList& GetVSSystemUniforms() const override { return m_VSUniformBuffers; }
		inline const ShaderUniformBufferList& GetPSSystemUniforms() const override { return m_PSUniformBuffers; }
		inline const ShaderUniformBufferDeclaration* GetVSUserUniformBuffer() const override { return m_VSUserUniformBuffer; }
		inline const ShaderUniformBufferDeclaration* GetPSUserUniformBuffer() const override { return m_PSUserUniformBuffer; }
		inline const ShaderResourceList& GetResources() const override { return m_Resources; }
	private:
		static ID3DBlob* Compile(const String& source, const String& profile, const String& main, D3DShaderErrorInfo& info);
		void Load(const String& source);

		String RemoveComments(const String& source);
		void Parse(const String& source);
		void ParseCBuffer(const String& block);
		void ParseTexture(const String& statement);
		void ParseSamplerState(const String& statement);
		void ParseStruct(const String& block);

		void CreateBuffers();
		ShaderStruct* FindStruct(const String& name);
	public:
		static bool TryCompile(const String& source, String& error);
		static bool TryCompileFromFile(const String& filepath, String& error);
		static D3DShader* FromFile(const String& name, const String& filepath, void* address);
		static D3DShader* FromSource(const String& name, const String& source);
	public:
		static const D3DShader* CurrentlyBound() { return s_CurrentlyBound; }
	};

} } }