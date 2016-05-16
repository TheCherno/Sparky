#include "Vector4.h"

#include "Vector2.h"

namespace SparkyCLI {

	Vector4::Vector4(sparky::maths::vec4* instance)
		: ManagedClass(instance)
	{
	}

	Vector4::Vector4()
		: ManagedClass()
	{
	}

	Vector4::Vector4(float x, float y, float z, float w)
	{
		m_Instance = new sparky::maths::vec4(x, y, z, w);
	}

	Vector4^ Vector4::Add(Vector4^ other)
	{
		return gcnew Vector4(&m_Instance->Add(*other->GetHandle()));
	}

	Vector4^ Vector4::Subtract(Vector4^ other)
	{
		return gcnew Vector4(&m_Instance->Subtract(*other->GetHandle()));
	}

	Vector4^ Vector4::Multiply(Vector4^ other)
	{
		return gcnew Vector4(&m_Instance->Multiply(*other->GetHandle()));
	}

	Vector4^ Vector4::Divide(Vector4^ other)
	{
		return gcnew Vector4(&m_Instance->Divide(*other->GetHandle()));
	}

	SparkyCLI::Vector4^ Vector4::operator+(Vector4^ left, Vector4^ right)
	{
		return left->Add(right);
	}

	SparkyCLI::Vector4^ Vector4::operator-(Vector4^ left, Vector4^ right)
	{
		return left->Subtract(right);
	}

	SparkyCLI::Vector4^ Vector4::operator*(Vector4^ left, Vector4^ right)
	{
		return left->Multiply(right);
	}

	SparkyCLI::Vector4^ Vector4::operator/(Vector4^ left, Vector4^ right)
	{
		return left->Divide(right);
	}

	bool Vector4::operator==(Vector4^ other)
	{
		return *m_Instance == *other->GetHandle();
	}

	bool Vector4::operator!=(Vector4^ other)
	{
		return *m_Instance != *other->GetHandle();
	}

	Vector4^ Vector4::operator+=(Vector4^ other)
	{
		return Add(other);
	}

	Vector4^ Vector4::operator-=(Vector4^ other)
	{
		return Subtract(other);
	}

	Vector4^ Vector4::operator*=(Vector4^ other)
	{
		return Multiply(other);
	}

	Vector4^ Vector4::operator/=(Vector4^ other)
	{
		return Divide(other);
	}

	System::String^ Vector4::ToString()
	{
		// TODO: Unimplemented
		return "Vector4::ToString(): Unimplemented";
	}

}