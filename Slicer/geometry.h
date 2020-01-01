#pragma once
#include <cmath>
#include <iostream>

template<unsigned int N>
class Vector
{
public:
	template<typename... Args>
	Vector<N>(Args... args) : elements{ args... } {
		static_assert(sizeof... (Args) == N, "Wrong number of elements supplied in constructor");
	}
	Vector<N>();

	Vector<N>& operator+=(const Vector<N>& rhs);
	Vector<N> operator+(const Vector<N>& rhs) const;
	Vector<N> operator-() const;
	Vector<N>& operator-=(const Vector<N>& rhs);
	Vector<N> operator-(const Vector<N>& rhs) const;
	Vector<N>& operator*=(float rhs);
	Vector<N> operator*(float rhs) const;
	friend Vector<N> operator*(float lhs, const Vector<N>& rhs);
	Vector<N>& operator/=(float rhs);
	Vector<N> operator/(float rhs) const;
	friend Vector<N> operator/(float lhs, const Vector<N>& rhs);

	float& operator[](unsigned int i);
	float operator[](unsigned int i) const;

	void normalize();
	float length() const;
	float dot(const Vector<N>& rhs) const;
private:
	float elements[N];
};

typedef Vector<3> Vector3d;
typedef Vector<2> Vector2d;

class Triangle
{
public:
	Vector3d normal;
	Vector3d vertices[3];
};

struct Plane {
	Vector3d point, normal;
};

struct Line {
	Vector3d point, direction;
};

template<unsigned int N>
inline Vector<N>::Vector()
{
	for (unsigned int i = 0; i < N; i++)
	{
		elements[i] = 0.0f;
	}
}

template<unsigned int N>
inline Vector<N>& Vector<N>::operator+=(const Vector<N>& rhs)
{
	for (unsigned int i = 0; i < N; i++)
	{
		elements[i] += rhs.elements[i];
	}
	return *this;
}

template<unsigned int N>
inline Vector<N> Vector<N>::operator+(const Vector<N>& rhs) const
{
	Vector<N> result(*this);
	return result += rhs;
}

template<unsigned int N>
inline Vector<N> Vector<N>::operator-() const
{
	Vector<N> result(*this);
	return result * -1.0f;
}

template<unsigned int N>
inline Vector<N>& Vector<N>::operator-=(const Vector<N>& rhs)
{
	return (*this) += -rhs;
}

template<unsigned int N>
inline Vector<N> Vector<N>::operator-(const Vector<N>& rhs) const
{
	Vector<N> result(*this);
	return result -= rhs;
}

template<unsigned int N>
inline Vector<N>& Vector<N>::operator*=(float rhs)
{
	for (unsigned int i = 0; i < N; i++)
	{
		elements[i] *= rhs;
	}
	return *this;
}

template<unsigned int N>
inline Vector<N> Vector<N>::operator*(float rhs) const
{
	Vector<N> result(*this);
	return result *= rhs;
}

template<unsigned int N>
inline Vector<N>& Vector<N>::operator/=(float rhs)
{
	return (*this) *= (1.0f / rhs);
}

template<unsigned int N>
inline Vector<N> Vector<N>::operator/(float rhs) const
{
	Vector<N> result(*this);
	return result /= rhs;
}

template<unsigned int N>
inline float& Vector<N>::operator[](unsigned int i)
{
	return elements[i];
}

template<unsigned int N>
inline float Vector<N>::operator[](unsigned int i) const
{
	return elements[i];
}

template<unsigned int N>
inline void Vector<N>::normalize()
{
	(*this) /= this->length();
}

template<unsigned int N>
inline float Vector<N>::length() const
{
	return std::sqrtf(this->dot(*this));
}

template<unsigned int N>
inline float Vector<N>::dot(const Vector<N> & rhs) const
{
	float result = 0.0f;
	for (unsigned int i = 0; i < N; i++)
	{
		result += this->elements[i] * rhs.elements[i];
	}

	return result;
}

template<unsigned int N>
inline std::ostream& operator<<(std::ostream & out, const Vector<N> & v) {
	out << "[";
	for (unsigned int i = 0; i < N; i++)
	{
		out << v[i];
		if (i < N - 1) {
			out << " ";
		}
	}
	out << "]";
	return out;
}

template<unsigned int N>
inline std::istream& operator>>(std::istream & in, Vector<N> & v) {
	for (unsigned int i = 0; i < N; i++)
	{
		in >> v[i];
	}
	return in;
}

template<unsigned int N>
inline Vector<N> operator*(float lhs, const Vector<N> & rhs) {
	Vector<N> result(*this);
	return result *= rhs;
}

template<unsigned int N>
inline Vector<N> operator/(float lhs, const Vector<N> & rhs) {
	Vector<N> result(*this);
	return result /= rhs;
}
