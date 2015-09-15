#include "Shader.h"

namespace SparkyCLI {

	Shader::Shader(sparky::graphics::Shader* instance)
		: ManagedClass(instance)
	{
	}

	Shader::Shader(System::String^ vertPath, System::String^ fragPath)
	{
		m_Instance = new sparky::graphics::Shader(string_to_char_array(vertPath), string_to_char_array(fragPath));
	}

	Shader::Shader(System::String^ name, System::String^ vertSrc, System::String^ fragSrc)
	{
		m_Instance = new sparky::graphics::Shader(string_to_char_array(name), string_to_char_array(vertSrc), string_to_char_array(fragSrc));
	}


	void Shader::SetUniform1f(System::String^ name, float value)
	{
		m_Instance->SetUniform1f(string_to_char_array(name), value);
	}

	void Shader::SetUniform1fv(System::String^ name, array<float>^ value, int count)
	{
		pin_ptr<float> p = &value[0];
		m_Instance->SetUniform1fv(string_to_char_array(name), p, count);
	}

	void Shader::SetUniform1i(System::String^ name, int value)
	{
		m_Instance->SetUniform1i(string_to_char_array(name), value);
	}

	void Shader::SetUniform1iv(System::String^ name, array<int>^ value, int count)
	{
		pin_ptr<int> p = &value[0];
		m_Instance->SetUniform1iv(string_to_char_array(name), p, count);
	}

	void Shader::SetUniform2f(System::String^ name, Vector2^ vector)
	{
		m_Instance->SetUniform2f(string_to_char_array(name), *vector->GetHandle());
	}

	void Shader::SetUniform3f(System::String^ name, Vector3^ vector)
	{
		m_Instance->SetUniform3f(string_to_char_array(name), *vector->GetHandle());
	}

	void Shader::SetUniform4f(System::String^ name, Vector4^ vector)
	{
		m_Instance->SetUniform4f(string_to_char_array(name), *vector->GetHandle());
	}

	void Shader::SetUniformMat4(System::String^ name, Matrix4^ matrix)
	{
		m_Instance->SetUniformMat4(string_to_char_array(name), *matrix->GetHandle());
	}

	void Shader::Bind()
	{
		m_Instance->Bind();
	}

	void Shader::Unbind()
	{
		m_Instance->Unbind();
	}

	Shader^ Shader::FromFile(System::String^ vertPath, System::String^ fragPath)
	{
		return gcnew Shader(sparky::graphics::Shader::FromFile(string_to_char_array(vertPath), string_to_char_array(fragPath)));
	}

	Shader^ Shader::FromSource(System::String^ vertSrc, System::String^ fragSrc)
	{
		return gcnew Shader(sparky::graphics::Shader::FromSource(string_to_char_array(vertSrc), string_to_char_array(fragSrc)));
	}

	Shader^ Shader::FromSource(System::String^ name, System::String^ vertSrc, System::String^ fragSrc)
	{
		return gcnew Shader(sparky::graphics::Shader::FromSource(string_to_char_array(name), string_to_char_array(vertSrc), string_to_char_array(fragSrc)));
	}

}