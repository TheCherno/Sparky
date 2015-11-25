#include "Shader.h"

#include <GL/glew.h>

namespace sparky { namespace graphics {

	Shader::Shader(const char* name, const char* vertSrc, const char* fragSrc)
		: m_Name(name), m_VertSrc(vertSrc), m_FragSrc(fragSrc)
	{
		m_ShaderID = Load(m_VertSrc, m_FragSrc);
	}

	Shader::Shader(const char* vertPath, const char* fragPath)
		: m_Name(vertPath), m_VertPath(vertPath), m_FragPath(fragPath)
	{
		std::string vertSourceString = read_file(m_VertPath);
		std::string fragSourceString = read_file(m_FragPath);

		m_VertSrc = vertSourceString.c_str();
		m_FragSrc = fragSourceString.c_str();
		m_ShaderID = Load(m_VertSrc, m_FragSrc);
	}

	Shader* Shader::FromFile(const char* vertPath, const char* fragPath)
	{
		return new Shader(vertPath, fragPath);
	}

	Shader* Shader::FromSource(const char* vertSrc, const char* fragSrc)
	{
		return new Shader(vertSrc, vertSrc, fragSrc);
	}

	Shader* Shader::FromSource(const char* name, const char* vertSrc, const char* fragSrc)
	{
		return new Shader(name, vertSrc, fragSrc);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderID);
	}

	GLuint Shader::Load(const char* vertSrc, const char* fragSrc)
	{
		GLuint program = glCreateProgram();
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex, 1, &vertSrc, NULL);
		glCompileShader(vertex);

		GLint result;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(vertex, length, &length, &error[0]);
			SPARKY_ERROR("Failed to compile vertex shader!");
			SPARKY_ERROR(&error[0]);
			SPARKY_ASSERT(false, "Failed to compile vertex shader!");
			glDeleteShader(vertex);
			return 0;
		}

		glShaderSource(fragment, 1, &fragSrc, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(fragment, length, &length, &error[0]);
			SPARKY_ERROR("Failed to compile fragment shader!");
			SPARKY_ERROR(&error[0]);
			SPARKY_ASSERT(false, "Failed to compile fragment shader!");
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

	GLint Shader::GetUniformLocation(const GLchar* name)
	{
		GLint result = glGetUniformLocation(m_ShaderID, name);
		if (result == -1)
			SPARKY_ERROR(m_Name, ": could not find uniform ", name, " in shader!");

		return result;
	}

	void Shader::SetUniform1f(const GLchar* name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1fv(const GLchar* name, float* value, int count)
	{
		glUniform1fv(GetUniformLocation(name), count, value);
	}

	void Shader::SetUniform1i(const GLchar* name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1iv(const GLchar* name, int* value, int count)
	{
		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void Shader::SetUniform2f(const GLchar* name, const maths::vec2& vector)
	{
		glUniform2f(GetUniformLocation(name), vector.x, vector.y);
	}

	void Shader::SetUniform3f(const GLchar* name, const maths::vec3& vector)
	{
		glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void Shader::SetUniform4f(const GLchar* name, const maths::vec4& vector)
	{
		glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void Shader::SetUniformMat4(const GLchar* name, const maths::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix.elements);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ShaderID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

} }