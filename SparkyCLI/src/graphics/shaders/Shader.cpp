#include "Shader.h"

namespace SparkyCLI {

    Shader::Shader(sp::graphics::API::Shader* instance)
        : ManagedClass(instance)
    {
    }

    Shader::Shader()
    {
        //TODO: Use static Shader::MakeFrom...()
        //m_Instance = new sp::graphics::API::Shader();
    }


    void Shader::SetUniform1f(System::String^ name, float value)
    {
        //m_Instance->SetUniform1f(string_to_char_array(name), value);
    }

    void Shader::SetUniform1fv(System::String^ name, array<float>^ value, int count)
    {
        pin_ptr<float> p = &value[0];
        //m_Instance->SetUniform1fv(string_to_char_array(name), p, count);
    }

    void Shader::SetUniform1i(System::String^ name, int value)
    {
        //m_Instance->SetUniform1i(string_to_char_array(name), value);
    }

    void Shader::SetUniform1iv(System::String^ name, array<int>^ value, int count)
    {
        pin_ptr<int> p = &value[0];
        //m_Instance->SetUniform1iv(string_to_char_array(name), p, count);
    }

    void Shader::SetUniform2f(System::String^ name, Vector2^ vector)
    {
        //m_Instance->SetUniform2f(string_to_char_array(name), *vector->GetHandle());
    }

    void Shader::SetUniform3f(System::String^ name, Vector3^ vector)
    {
        //m_Instance->SetUniform3f(string_to_char_array(name), *vector->GetHandle());
    }

    void Shader::SetUniform4f(System::String^ name, Vector4^ vector)
    {
        //m_Instance->SetUniform4f(string_to_char_array(name), *vector->GetHandle());
    }

    void Shader::SetUniformMat4(System::String^ name, Matrix4^ matrix)
    {
        //m_Instance->SetUniformMat4(string_to_char_array(name), *matrix->GetHandle());
    }

    void Shader::Bind()
    {
        m_Instance->Bind();
    }

    void Shader::Unbind()
    {
        m_Instance->Unbind();
    }

    Shader^ Shader::FromFile(System::String^ name, System::String^ filePath)
    {
        return gcnew Shader(sp::graphics::API::Shader::CreateFromFile(string_to_char_array(name), string_to_char_array(filePath)));
    }

    Shader^ Shader::FromSource(System::String^ name, System::String^ src)
    {
        return gcnew Shader(sp::graphics::API::Shader::CreateFromSource(string_to_char_array(name), string_to_char_array(src)));
    }

}