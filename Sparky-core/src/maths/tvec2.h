#pragma once

#include <iostream>

namespace sparky { namespace maths {

	template <class T>
	struct tvec2
	{
		T x, y;

		tvec2<T>();
		tvec2<T>(const T& x, const T& y);

		tvec2<T>& add(const tvec2<T>& other);
		tvec2<T>& subtract(const tvec2<T>& other);
		tvec2<T>& multiply(const tvec2<T>& other);
		tvec2<T>& divide(const tvec2<T>& other);

		friend tvec2<T> operator+(tvec2<T> left, const tvec2<T>& right);
		friend tvec2<T> operator-(tvec2<T> left, const tvec2<T>& right);
		friend tvec2<T> operator*(tvec2<T> left, const tvec2<T>& right);
		friend tvec2<T> operator/(tvec2<T> left, const tvec2<T>& right);

		bool operator==(const tvec2<T>& other);
		bool operator!=(const tvec2<T>& other);

		tvec2<T>& operator+=(const tvec2<T>& other);
		tvec2<T>& operator-=(const tvec2<T>& other);
		tvec2<T>& operator*=(const tvec2<T>& other);
		tvec2<T>& operator/=(const tvec2<T>& other);

		friend std::ostream& operator<<(std::ostream& stream, const tvec2& vector);
	};

	template <class T>
	tvec2<T>::tvec2()
	{
		x = 0;
		y = 0;
	}

	template <class T>
	tvec2<T>::tvec2(const T& x, const T& y)
	{
		this->x = x;
		this->y = y;
	}

	template <class T>
	tvec2<T>& tvec2<T>::add(const tvec2<T>& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	template <class T>
	tvec2<T>& tvec2<T>::subtract(const tvec2<T>& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}
	
	template <class T>
	tvec2<T>& tvec2<T>::multiply(const tvec2<T>& other)
	{
		x *= other.x;
		y *= other.y;

		return *this;
	}

	template <class T>
	tvec2<T>& tvec2<T>::divide(const tvec2<T>& other)
	{
		x /= other.x;
		y /= other.y;

		return *this;
	}

	template <class T>
	tvec2<T> operator+(tvec2<T> left, const tvec2<T>& right)
	{
		return left.add(right);
	}

	template <class T>
	tvec2<T> operator-(tvec2<T> left, const tvec2<T>& right)
	{
		return left.subtract(right);
	}

	template <class T>
	tvec2<T> operator*(tvec2<T> left, const tvec2<T>& right)
	{
		return left.multiply(right);
	}

	template <class T>
	tvec2<T> operator/(tvec2<T> left, const tvec2<T>& right)
	{
		return left.divide(right);
	}

	template <class T>
	tvec2<T>& tvec2<T>::operator+=(const tvec2<T>& other)
	{
		return add(other);
	}

	template <class T>
	tvec2<T>& tvec2<T>::operator-=(const tvec2<T>& other)
	{
		return subtract(other);
	}

	template <class T>
	tvec2<T>& tvec2<T>::operator*=(const tvec2<T>& other)
	{
		return multiply(other);
	}

	template <class T>
	tvec2<T>& tvec2<T>::operator/=(const tvec2<T>& other)
	{
		return divide(other);
	}

	template <class T>
	bool tvec2<T>::operator==(const tvec2<T>& other)
	{
		return x == other.x && y == other.y;
	}

	template <class T>
	bool tvec2<T>::operator!=(const tvec2<T>& other)
	{
		return !(*this == other);
	}

	template <class T>
	std::ostream& operator<<(std::ostream& stream, const tvec2<T>& vector)
	{
		stream << "tvec2: (" << vector.x << ", " << vector.y << ")";
		return stream;
	}

} }