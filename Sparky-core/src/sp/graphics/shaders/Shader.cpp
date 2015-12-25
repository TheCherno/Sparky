#include "sp/sp.h"
#include "Shader.h"

#include <sp/utils/StringUtils.h>
#include <GL/glew.h>

namespace sp { namespace graphics {

	Shader::Shader(const String& name, const String& source)
		: m_Name(name), m_Source(source)
	{
		String** shaders = new String*[2];
		shaders[0] = &m_VertexSource;
		shaders[1] = &m_FragmentSource;

		PreProcess(source, shaders);
		m_ShaderID = Load(m_VertexSource, m_FragmentSource);
		ResolveUniforms();
		ValidateUniforms();
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderID);
	}

	Shader* Shader::FromFile(const String& name, const String& filepath)
	{
		String shader = utils::ReadFile(filepath);
		return new Shader(name, shader);
	}

	Shader* Shader::FromSource(const String& name, const String& source)
	{
		return new Shader(name, source);
	}

	void Shader::PreProcess(const String& source, String** shaders)
	{
		ShaderType type = ShaderType::UNKNOWN;

		std::vector<String> lines = utils::SplitString(source, '\n');
		for (uint i = 0; i < lines.size(); i++)
		{
			const char* str = lines[i].c_str();
			if (strstr(str, "#shader"))
			{
				if (strstr(str, "vertex"))
					type = ShaderType::VERTEX;
				else if (strstr(str, "fragment"))
					type = ShaderType::FRAGMENT;
			}
			else if (type != ShaderType::UNKNOWN)
			{
				shaders[(int)type - 1]->append(str);
				shaders[(int)type - 1]->append("\n");
			}
		}
		ParseUniforms(lines);
	}

	void Shader::ParseUniforms(const std::vector<String>& lines)
	{
		for (uint i = 0; i < lines.size(); i++)
		{
			const char* str = lines[i].c_str();
			if (strstr(str, "uniform"))
			{
				std::vector<String> line = utils::SplitString(str, ' ');
				for (uint i = 0; i < line.size(); i++)
				{
					// TODO: Precision
					String& token = line[i];
					ShaderUniformDeclaration::Type type = GetUniformTypeFromString(token);
					if (type != ShaderUniformDeclaration::Type::NONE)
					{
						String& nextToken = line[i + 1];
						String name = nextToken;
						if (name[name.size() - 1] == ';')
							name = nextToken.substr(0, nextToken.size() - 1);

						uint count = 1;

						// Uniform arrays
						uint arrayToken = nextToken.find('[');
						if (arrayToken != String::npos)
						{
							name = name.substr(0, arrayToken - 1);
							uint arrayEnd = nextToken.find(']');
							SP_ASSERT(arrayEnd != String::npos);
							count = atoi(nextToken.substr(arrayToken + 1, arrayEnd - arrayToken - 1).c_str());
						}

						ShaderUniformDeclaration* uniform = new ShaderUniformDeclaration(type, name, this, count);
						m_Uniforms.push_back(uniform);
						break;
					}
				}
			}
		}
	}

	ShaderUniformDeclaration::Type Shader::GetUniformTypeFromString(const String& token)
	{
		if (token == "float") return ShaderUniformDeclaration::Type::FLOAT32;
		if (token == "int") return ShaderUniformDeclaration::Type::INT32;
		if (token == "vec2") return ShaderUniformDeclaration::Type::VEC2;
		if (token == "vec3") return ShaderUniformDeclaration::Type::VEC3;
		if (token == "vec4") return ShaderUniformDeclaration::Type::VEC4;
		if (token == "mat3") return ShaderUniformDeclaration::Type::MAT3;
		if (token == "mat4") return ShaderUniformDeclaration::Type::MAT4;
		if (token == "sampler2D") return ShaderUniformDeclaration::Type::SAMPLER2D;

		return ShaderUniformDeclaration::Type::NONE;
	}

	GLuint Shader::Load(const String& vertSrc, const String& fragSrc)
	{
		const char* vertexSource = vertSrc.c_str();
		const char* fragmentSource = fragSrc.c_str();

		GLuint program = glCreateProgram();
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);

		GLint result;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(vertex, length, &length, &error[0]);
			SP_ERROR("Failed to compile vertex shader!");
			SP_ERROR(&error[0]);
			SP_ASSERT(false, "Failed to compile vertex shader!");
			glDeleteShader(vertex);
			return 0;
		}

		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(fragment, length, &length, &error[0]);
			SP_ERROR("Failed to compile fragment shader!");
			SP_ERROR(&error[0]);
			SP_ASSERT(false, "Failed to compile fragment shader!");
			glDeleteShader(fragment);
			return 0;
		}

		glAttachShader(program, vertex);
		glAttachShader(program, fragment);

		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return program;
	}

	void Shader::ResolveUniforms()
	{
		uint offset = 0;
		for (uint i = 0; i < m_Uniforms.size(); i++)
		{
			ShaderUniformDeclaration* uniform = m_Uniforms[i];
			uniform->m_Offset = offset;
			uniform->m_Location = GetUniformLocation(uniform->m_Name);

			offset += uniform->GetSize();
		}
	}

	void Shader::ValidateUniforms()
	{
		if (!HasUniform(SHADER_UNIFORM_PROJECTION_MATRIX_NAME))
			SP_WARN(m_Name, " shader does not contain pr_matrix uniform.");
		if (!HasUniform(SHADER_UNIFORM_VIEW_MATRIX_NAME))
			SP_WARN(m_Name, " shader does not contain vw_matrix uniform.");
		if (!HasUniform(SHADER_UNIFORM_MODEL_MATRIX_NAME))
			SP_WARN(m_Name, " shader does not contain ml_matrix uniform.");

		SP_INFO(m_Name, " shader successfully validated.");
	}

	bool Shader::IsSystemUniform(ShaderUniformDeclaration* uniform) const
	{
		// TODO: Optimize with naming convention for system uniforms

		if (uniform->GetName() == SHADER_UNIFORM_PROJECTION_MATRIX_NAME)
			return true;
		if (uniform->GetName() == SHADER_UNIFORM_VIEW_MATRIX_NAME)
			return true;
		if (uniform->GetName() == SHADER_UNIFORM_MODEL_MATRIX_NAME)
			return true;

		return false;
	}

	GLint Shader::GetUniformLocation(const String& name)
	{
		GLint result = glGetUniformLocation(m_ShaderID, name.c_str());
		if (result == -1)
			SP_ERROR(m_Name, ": could not find uniform ", name, " in shader!");

		return result;
	}

	ShaderUniformDeclaration* Shader::FindUniformDeclaration(const String& name)
	{
		for (uint i = 0; i < m_Uniforms.size(); i++)
		{
			if (m_Uniforms[i]->GetName() == name)
				return m_Uniforms[i];
		}
		return nullptr;
	}

	void Shader::SetUniform1f(const String& name, float value)
	{
		SetUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1fv(const String& name, float* value, int count)
	{
		SetUniform1fv(GetUniformLocation(name), value, count);
	}

	void Shader::SetUniform1i(const String& name, int value)
	{
		SetUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1iv(const String& name, int* value, int count)
	{
		SetUniform1iv(GetUniformLocation(name), value, count);
	}

	void Shader::SetUniform2f(const String& name, const maths::vec2& vector)
	{
		SetUniform2f(GetUniformLocation(name), vector);
	}

	void Shader::SetUniform3f(const String& name, const maths::vec3& vector)
	{
		SetUniform3f(GetUniformLocation(name), vector);
	}

	void Shader::SetUniform4f(const String& name, const maths::vec4& vector)
	{
		SetUniform4f(GetUniformLocation(name), vector);
	}

	void Shader::SetUniformMat4(const String& name, const maths::mat4& matrix)
	{
		SetUniformMat4(GetUniformLocation(name), matrix);
	}

	void Shader::SetUniform(const String& name, byte* data)
	{
		ShaderUniformDeclaration* uniform = FindUniformDeclaration(name);
		if (!uniform)
		{
			SP_ERROR("Cannot find uniform in ", m_Name, " shader with name '", name, "'");
			return;
		}
		ResolveAndSetUniform(uniform, data, 0);
	}

	void Shader::ResolveAndSetUniform(ShaderUniformDeclaration* uniform, byte* data, int offset)
	{
		switch (uniform->GetType())
		{
			case ShaderUniformDeclaration::Type::FLOAT32:
				SetUniform1f(uniform->GetLocation(), *(float*)&data[offset]);
				break;
			case ShaderUniformDeclaration::Type::SAMPLER2D:
			case ShaderUniformDeclaration::Type::INT32:
				SetUniform1i(uniform->GetLocation(), *(int*)&data[offset]);
				break;
			case ShaderUniformDeclaration::Type::VEC2:
				SetUniform2f(uniform->GetLocation(), *(maths::vec2*)&data[offset]);
				break;
			case ShaderUniformDeclaration::Type::VEC3:
				SetUniform3f(uniform->GetLocation(), *(maths::vec3*)&data[offset]);
				break;
			case ShaderUniformDeclaration::Type::VEC4:
				SetUniform4f(uniform->GetLocation(), *(maths::vec4*)&data[offset]);
				break;
			case ShaderUniformDeclaration::Type::MAT3:
				// TODO: SetUniformMat3(uniform->GetLocation(), *(maths::mat3*)&data[offset]);
				break;
			case ShaderUniformDeclaration::Type::MAT4:
				SetUniformMat4(uniform->GetLocation(), *(maths::mat4*)&data[offset]);
				break;
			default:
				SP_ASSERT(false, "Unknown type!");
		}
	}

	void Shader::ResolveAndSetUniform(uint index, byte* data)
	{
		ShaderUniformDeclaration* uniform = m_Uniforms[index];
		if (IsSystemUniform(uniform))
			return;
		ResolveAndSetUniform(uniform, data, uniform->GetOffset());
	}

	void Shader::ResolveAndSetUniforms(byte* data, uint size)
	{
		const std::vector<ShaderUniformDeclaration*>& uniforms = m_Uniforms;
		for (uint i = 0; i < uniforms.size(); i++)
			ResolveAndSetUniform(i, data);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ShaderID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	bool Shader::HasUniform(const String& name) const
	{
		for (uint i = 0; i < m_Uniforms.size(); i++)
		{
			if (m_Uniforms[i]->m_Name == name)
				return true;
		}
		return false;
	}

	ShaderUniformDeclaration* Shader::GetUniformDeclaration(uint location)
	{
		for (uint i = 0; i < m_Uniforms.size(); i++)
		{
			if (m_Uniforms[i]->GetLocation() == location)
				return m_Uniforms[i];
		}
		return nullptr;
	}

	void Shader::SetUniform1f(uint location, float value)
	{
		glUniform1f(location, value);
	}

	void Shader::SetUniform1fv(uint location, float* value, int count)
	{
		glUniform1fv(location, count, value);
	}

	void Shader::SetUniform1i(uint location, int value)
	{
		glUniform1i(location, value);
	}

	void Shader::SetUniform1iv(uint location, int* value, int count)
	{
		glUniform1iv(location, count, value);
	}

	void Shader::SetUniform2f(uint location, const maths::vec2& vector)
	{
		glUniform2f(location, vector.x, vector.y);
	}

	void Shader::SetUniform3f(uint location, const maths::vec3& vector)
	{
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void Shader::SetUniform4f(uint location, const maths::vec4& vector)
	{
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void Shader::SetUniformMat4(uint location, const maths::mat4& matrix)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix.elements);
	}

} }