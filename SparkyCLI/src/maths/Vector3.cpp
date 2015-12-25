#include "Vector3.h"

#include "Vector2.h"

namespace SparkyCLI {

	Vector3::Vector3(sparky::maths::vec3* instance)
		: ManagedClass(instance)
	{
	}

	Vector3::Vector3()
		: ManagedClass()
	{
	}

	Vector3::Vector3(float x, float y, float z)
	{
		m_Instance = new sparky::maths::vec3(x, y, z);
	}

	Vector3::Vector3(Vector2^ vector)
	{
		m_Instance = new sparky::maths::vec3(*vector->GetHandle());
	}

	Vector3^ Vector3::Add(Vector3^ other)
	{
		return gcnew Vector3(&m_Instance->Add(*other->GetHandle()));
	}

	Vector3^ Vector3::Subtract(Vector3^ other)
	{
		return gcnew Vector3(&m_Instance->Subtract(*other->GetHandle()));
	}

	Vector3^ Vector3::Multiply(Vector3^ other)
	{
		return gcnew Vector3(&m_Instance->Multiply(*other->GetHandle()));
	}

	Vector3^ Vector3::Divide(Vector3^ other)
	{
		return gcnew Vector3(&m_Instance->Divide(*other->GetHandle()));
	}

	SparkyCLI::Vector3^ Vector3::operator+(Vector3^ left, Vector3^ right)
	{
		return left->Add(right);
	}

	SparkyCLI::Vector3^ Vector3::operator-(Vector3^ left, Vector3^ right)
	{
		return left->Subtract(right);
	}

	SparkyCLI::Vector3^ Vector3::operator*(Vector3^ left, Vector3^ right)
	{
		return left->Multiply(right);
	}

	SparkyCLI::Vector3^ Vector3::operator/(Vector3^ left, Vector3^ right)
	{
		return left->Divide(right);
	}

	bool Vector3::operator==(Vector3^ other)
	{
		return *m_Instance == *other->GetHandle();
	}

	bool Vector3::operator!=(Vector3^ other)
	{
		return *m_Instance != *other->GetHandle();
	}

	Vector3^ Vector3::operator+=(Vector3^ other)
	{
		return Add(other);
	}

	Vector3^ Vector3::operator-=(Vector3^ other)
	{
		return Subtract(other);
	}

	Vector3^ Vector3::operator*=(Vector3^ other)
	{
		return Multiply(other);
	}

	Vector3^ Vector3::operator/=(Vector3^ other)
	{
		return Divide(other);
	}

	float Vector3::Distance(Vector3^ other)
	{
		return m_Instance->Distance(*other->GetHandle());
	}

	System::String^ Vector3::ToString()
	{
		// TODO: Unimplemented
		return "Vector3::ToString(): Unimplemented";
	}

}