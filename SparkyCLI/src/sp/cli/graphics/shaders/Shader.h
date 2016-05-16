#pragma once

#include "../../SparkyCLI.h"
#include "../../maths/Vector2.h"
#include "../../maths/Vector3.h"
#include "../../maths/Matrix4.h"

#include <graphics/shaders/Shader.h>

namespace SparkyCLI {

	public ref class Shader : public ManagedClass<sparky::graphics::Shader>
	{
	private:
		Shader(sparky::graphics::Shader* instance);
	public:
		Shader(System::String^ name, System::String^ vertSrc, System::String^ fragSrc);
		Shader(System::String^ vertPath, System::String^ fragPath);

		void SetUniform1f(System::String^ name, float value);
		void SetUniform1fv(System::String^ name, array<float>^ value, int count);
		void SetUniform1i(System::String^ name, int value);
		void SetUniform1iv(System::String^ name, array<int>^ value, int count);
		void SetUniform2f(System::String^ name, Vector2^ vector);
		void SetUniform3f(System::String^ name, Vector3^ vector);
		void SetUniform4f(System::String^ name, Vector4^ vector);
		void SetUniformMat4(System::String^ name, Matrix4^ matrix);

		void Bind();
		void Unbind();

	public:
		static Shader^ FromFile(System::String^ vertPath, System::String^ fragPath);
		static Shader^ FromSource(System::String^ vertSrc, System::String^ fragSrc);
		static Shader^ FromSource(System::String^ name, System::String^ vertSrc, System::String^ fragSrc);
	};

}