#include "Vector2.h"

namespace SparkyCLI {

	Vector2::Vector2(sparky::maths::vec2* instance)
		: ManagedClass(instance)
	{
	}

	Vector2::Vector2()
		: ManagedClass()
	{
	}

	Vector2::Vector2(float x, float y)
	{
		m_Instance = new sparky::maths::vec2(x, y);
	}

	Vector2::Vector2(Vector3^ vector)
	{
		m_Instance = new sparky::maths::vec2(*vector->GetHandle());
	}

	Vector2^ Vector2::Add(Vector2^ other)
	{
		return gcnew Vector2(&m_Instance->Add(*other->GetHandle()));
	}

	Vector2^ Vector2::Subtract(Vector2^ other)
	{
		return gcnew Vector2(&m_Instance->Subtract(*other->GetHandle()));
	}

	Vector2^ Vector2::Multiply(Vector2^ other)
	{
		return gcnew Vector2(&m_Instance->Multiply(*other->GetHandle()));
	}

	Vector2^ Vector2::Divide(Vector2^ other)
	{
		return gcnew Vector2(&m_Instance->Divide(*other->GetHandle()));
	}

	SparkyCLI::Vector2^ Vector2::operator+(Vector2^ left, Vector2^ right)
	{
		return left->Add(right);
	}

	SparkyCLI::Vector2^ Vector2::operator-(Vector2^ left, Vector2^ right)
	{
		return left->Subtract(right);
	}

	SparkyCLI::Vector2^ Vector2::operator*(Vector2^ left, Vector2^ right)
	{
		return left->Multiply(right);
	}

	SparkyCLI::Vector2^ Vector2::operator/(Vector2^ left, Vector2^ right)
	{
		return left->Divide(right);
	}

	SparkyCLI::Vector2^ Vector2::operator+(Vector2^ left, float value)
	{
		return gcnew Vector2(left->x + value, left->y + value);
	}

	SparkyCLI::Vector2^ Vector2::operator*(Vector2^ left, float value)
	{
		return gcnew Vector2(left->x * value, left->y * value);
	}

	bool Vector2::operator==(Vector2^ other)
	{
		return *m_Instance == *other->GetHandle();
	}

	bool Vector2::operator!=(Vector2^ other)
	{
		return *m_Instance != *other->GetHandle();
	}

	Vector2^ Vector2::operator+=(Vector2^ other)
	{
		return Add(other);
	}

	Vector2^ Vector2::operator-=(Vector2^ other)
	{
		return Subtract(other);
	}

	Vector2^ Vector2::operator*=(Vector2^ other)
	{
		return Multiply(other);
	}

	Vector2^ Vector2::operator/=(Vector2^ other)
	{
		return Divide(other);
	}

	float Vector2::Magnitude()
	{
		return m_Instance->Magnitude();
	}

	SparkyCLI::Vector2^ Vector2::Normalise()
	{
		return gcnew Vector2(&m_Instance->Normalise());
	}

	float Vector2::Distance(Vector2^ other)
	{
		return m_Instance->Distance(*other->GetHandle());
	}

	float Vector2::Dot(Vector2^ other)
	{
		return m_Instance->Dot(*other->GetHandle());
	}

	System::String^ Vector2::ToString()
	{
		// TODO: Unimplemented
		return "Vector2::ToString(): Unimplemented";
	}

}