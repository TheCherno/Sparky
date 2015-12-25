#pragma once

#include "../SparkyCLI.h"

#include <maths/vec3.h>

namespace SparkyCLI {

	ref class Vector2;

	public ref class Vector3 : public ManagedClass<sparky::maths::vec3>
	{
	private:
		Vector3(sparky::maths::vec3* instance);
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

		property float z
		{
			float get()
			{
				return m_Instance->z;
			}
			void set(float value)
			{
				m_Instance->z = value;
			}
		}

		Vector3();
		Vector3(float x, float y, float z);
		Vector3(Vector2^ vector);

		Vector3^ Add(Vector3^ other);
		Vector3^ Subtract(Vector3^ other);
		Vector3^ Multiply(Vector3^ other);
		Vector3^ Divide(Vector3^ other);

		static Vector3^ operator+(Vector3^ left, Vector3^ right);
		static Vector3^ operator-(Vector3^ left, Vector3^ right);
		static Vector3^ operator*(Vector3^ left, Vector3^ right);
		static Vector3^ operator/(Vector3^ left, Vector3^ right);

		bool operator==(Vector3^ other);
		bool operator!=(Vector3^ other);

		Vector3^ operator+=(Vector3^ other);
		Vector3^ operator-=(Vector3^ other);
		Vector3^ operator*=(Vector3^ other);
		Vector3^ operator/=(Vector3^ other);

		float Distance(Vector3^ other);

		System::String^ ToString() override;
	};

}