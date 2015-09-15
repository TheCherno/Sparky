#pragma once

#include "../SparkyCLI.h"
#include "Vector3.h"

#include <maths/vec2.h>

namespace SparkyCLI {

	public ref class Vector2 : public ManagedClass<sparky::maths::vec2>
	{
	private:
		Vector2(sparky::maths::vec2* instance);
	public:
		property float x
		{
			float get()
			{
				return m_Instance->x;
			}
			void set(float value)
			{
				m_Instance->x = value;
			}
		}

		property float y
		{
			float get()
			{
				return m_Instance->y;
			}
			void set(float value)
			{
				m_Instance->y = value;
			}
		}

		Vector2();
		Vector2(float x, float y);
		Vector2(Vector3^ vector);

		Vector2^ Add(Vector2^ other);
		Vector2^ Subtract(Vector2^ other);
		Vector2^ Multiply(Vector2^ other);
		Vector2^ Divide(Vector2^ other);

		static Vector2^ operator+(Vector2^ left, Vector2^ right);
		static Vector2^ operator-(Vector2^ left, Vector2^ right);
		static Vector2^ operator*(Vector2^ left, Vector2^ right);
		static Vector2^ operator/(Vector2^ left, Vector2^ right);

		static Vector2^ operator+(Vector2^ left, float value);
		static Vector2^ operator*(Vector2^ left, float value);

		bool operator==(Vector2^ other);
		bool operator!=(Vector2^ other);

		Vector2^ operator+=(Vector2^ other);
		Vector2^ operator-=(Vector2^ other);
		Vector2^ operator*=(Vector2^ other);
		Vector2^ operator/=(Vector2^ other);

		float Magnitude();
		Vector2^ Normalise();
		float Distance(Vector2^ other);
		float Dot(Vector2^ other);

		System::String^ ToString() override;
	};

}