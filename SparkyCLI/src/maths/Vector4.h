#pragma once

#include "../SparkyCLI.h"

#include <maths/vec4.h>

namespace SparkyCLI {

	ref class Vector2;

	public ref class Vector4 : public ManagedClass<sparky::maths::vec4>
	{
	private:
		Vector4(sparky::maths::vec4* instance);
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

		property float w
		{
			float get()
			{
				return m_Instance->w;
			}
			void set(float value)
			{
				m_Instance->w = value;
			}
		}

		Vector4();
		Vector4(float x, float y, float z, float w);

		Vector4^ Add(Vector4^ other);
		Vector4^ Subtract(Vector4^ other);
		Vector4^ Multiply(Vector4^ other);
		Vector4^ Divide(Vector4^ other);

		static Vector4^ operator+(Vector4^ left, Vector4^ right);
		static Vector4^ operator-(Vector4^ left, Vector4^ right);
		static Vector4^ operator*(Vector4^ left, Vector4^ right);
		static Vector4^ operator/(Vector4^ left, Vector4^ right);

		bool operator==(Vector4^ other);
		bool operator!=(Vector4^ other);

		Vector4^ operator+=(Vector4^ other);
		Vector4^ operator-=(Vector4^ other);
		Vector4^ operator*=(Vector4^ other);
		Vector4^ operator/=(Vector4^ other);

		System::String^ ToString() override;
	};

}