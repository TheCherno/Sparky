#include "Matrix4.h"

namespace SparkyCLI {

	Matrix4::Matrix4(sparky::maths::mat4* instance)
		: ManagedClass(instance)
	{
	}

	Matrix4::Matrix4()
		: ManagedClass()
	{
	}

	Matrix4::Matrix4(float diagonal)
	{
		m_Instance = new sparky::maths::mat4(diagonal);
	}

	Matrix4^ Matrix4::Identity()
	{
		return gcnew Matrix4(&sparky::maths::mat4::Identity());
	}

	Matrix4^ Matrix4::Multiply(Matrix4^ other)
	{
		return gcnew Matrix4(&m_Instance->Multiply(*other->GetHandle()));
	}

	Matrix4^ Matrix4::operator*(Matrix4^ left, Matrix4^ right)
	{
		return left->Multiply(right);
	}

	Matrix4^ Matrix4::operator*=(Matrix4^ other)
	{
		return Multiply(other);
	}

	Vector3^ Matrix4::Multiply(Vector3^ other)
	{
		sparky::maths::vec3 result = m_Instance->Multiply(*other->GetHandle());
		return gcnew Vector3(result.x, result.y, result.z);
	}

	Vector3^ Matrix4::operator*(Matrix4^ left, Vector3^ right)
	{
		return left->Multiply(right);
	}

	Vector4^ Matrix4::Multiply(Vector4^ other)
	{
		sparky::maths::vec4 result = m_Instance->Multiply(*other->GetHandle());
		return gcnew Vector4(result.x, result.y, result.z, result.w);
	}

	Vector4^ Matrix4::operator*(Matrix4^ left, Vector4^ right)
	{
		return left->Multiply(right);
	}

	Matrix4^ Matrix4::Invert()
	{
		return gcnew Matrix4(&m_Instance->Invert());
	}

	Matrix4^ Matrix4::Orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		return gcnew Matrix4(&sparky::maths::mat4::Orthographic(left, right, bottom, top, near, far));
	}

	Matrix4^ Matrix4::Perspective(float fov, float aspectRatio, float near, float far)
	{
		return gcnew Matrix4(&sparky::maths::mat4::Perspective(fov, aspectRatio, near, far));
	}

	Matrix4^ Matrix4::Translate(Vector3^ translation)
	{
		return gcnew Matrix4(&sparky::maths::mat4::Translate(*translation->GetHandle()));
	}

	Matrix4^ Matrix4::Rotate(float angle, Vector3^ axis)
	{
		return gcnew Matrix4(&sparky::maths::mat4::Rotate(angle, *axis->GetHandle()));
	}

	Matrix4^ Matrix4::Scale(Vector3^ scale)
	{
		return gcnew Matrix4(&sparky::maths::mat4::Scale(*scale->GetHandle()));
	}

	Matrix4^ Matrix4::Invert(Matrix4^ matrix)
	{
		return gcnew Matrix4(&sparky::maths::mat4::Invert(*matrix->GetHandle()));
	}

	System::String^ Matrix4::ToString()
	{
		// TODO: Unimplemented
		return "Vector4::ToString(): Unimplemented";
	}

}