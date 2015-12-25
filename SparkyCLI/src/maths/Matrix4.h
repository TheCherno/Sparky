#pragma once

#include "../SparkyCLI.h"
#include "Vector3.h"
#include "Vector4.h"

#include <maths/mat4.h>

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

namespace SparkyCLI {

	ref class Vector2;

	public ref class Matrix4 : public ManagedClass<sparky::maths::mat4>
	{
	private:
		Matrix4(sparky::maths::mat4* instance);
	public:

		Matrix4();
		Matrix4(float diagonal);
		static Matrix4^ Identity();

		Matrix4^ Multiply(Matrix4^ other);
		static Matrix4^ operator*(Matrix4^ left, Matrix4^ right);
		Matrix4^ operator*=(Matrix4^ other);

		Vector3^ Multiply(Vector3^ other);
		static Vector3^ operator*(Matrix4^ left, Vector3^ right);

		Vector4^ Multiply(Vector4^ other);
		static Vector4^ operator*(Matrix4^ left, Vector4^ right);

		Matrix4^ Invert();

		static Matrix4^ Orthographic(float left, float right, float bottom, float top, float near, float far);
		static Matrix4^ Perspective(float fov, float aspectRatio, float near, float far);

		static Matrix4^ Translate(Vector3^ translation);
		static Matrix4^ Rotate(float angle, Vector3^ axis);
		static Matrix4^ Scale(Vector3^ scale);
		static Matrix4^ Invert(Matrix4^ matrix);

		System::String^ ToString() override;
	};

}