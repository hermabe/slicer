#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include <array>

template<typename T, unsigned int N>
class Vector
{
public:
	template<typename... Args>
	Vector<T, N>(Args... args) : elements{ args... } {
		static_assert(sizeof... (Args) == N, "Wrong number of elements supplied in constructor");
	}
	Vector<T, N>();
	template<unsigned int M>
	Vector<T, N>(const Vector<T, M>& rhs);

	Vector<T, N>& operator+=(const Vector<T, N>& rhs);
	Vector<T, N> operator+(const Vector<T, N>& rhs) const;
	Vector<T, N> operator-() const;
	Vector<T, N>& operator-=(const Vector<T, N>& rhs);
	Vector<T, N> operator-(const Vector<T, N>& rhs) const;
	Vector<T, N>& operator*=(T rhs);
	Vector<T, N> operator*(T rhs) const;
	friend Vector<T, N> operator*(T lhs, const Vector<T, N>& rhs);
	Vector<T, N>& operator/=(T rhs);
	Vector<T, N> operator/(T rhs) const;
	friend Vector<T, N> operator/(T lhs, const Vector<T, N>& rhs);

	T& operator[](unsigned int i);
	T operator[](unsigned int i) const;

	void normalize();
	T length() const;
	T dot(const Vector<T, N>& rhs) const;
private:
	T elements[N];
};

typedef Vector<float, 3> Vector3d;
typedef Vector<float, 2> Vector2d;

class Triangle
{
public:
	Vector3d normal;
	//Vector3d vertices[3];
	std::array<Vector3d, 3> vertices;
};

struct Plane {
	Vector3d point, normal;
};

struct Line {
	Vector3d point, direction;
};

struct Edge {
	Vector2d start, end, normal;
};

struct Polygon {
	std::vector<Vector2d> vertices;
};

Vector3d intersection(const Plane& plane, const Line& line);

template<typename T, unsigned int N>
inline Vector<T, N>::Vector()
{
	for (unsigned int i = 0; i < N; i++)
	{
		elements[i] = 0;
	}
}

template<unsigned int a, unsigned int b>
struct min {
	enum { value = a < b ? a : b };
};

template<typename T, unsigned int N>
template<unsigned int M>
inline Vector<T, N>::Vector(const Vector<T, M>& rhs) : Vector<T, N>()
{
	for (unsigned int i = 0; i < min<N, M>::value; i++)
	{
		this->elements[i] = rhs[i];
	}
}

template<typename T, unsigned int N>
inline Vector<T, N>& Vector<T, N>::operator+=(const Vector<T, N>& rhs)
{
	for (unsigned int i = 0; i < N; i++)
	{
		elements[i] += rhs.elements[i];
	}
	return *this;
}

template<typename T, unsigned int N>
inline Vector<T, N> Vector<T, N>::operator+(const Vector<T, N>& rhs) const
{
	Vector<T, N> result(*this);
	return result += rhs;
}

template<typename T, unsigned int N>
inline Vector<T, N> Vector<T, N>::operator-() const
{
	Vector<T, N> result(*this);
	return result * -1.0f;
}

template<typename T, unsigned int N>
inline Vector<T, N>& Vector<T, N>::operator-=(const Vector<T, N>& rhs)
{
	return (*this) += -rhs;
}

template<typename T, unsigned int N>
inline Vector<T, N> Vector<T, N>::operator-(const Vector<T, N>& rhs) const
{
	Vector<T, N> result(*this);
	return result -= rhs;
}

template<typename T, unsigned int N>
inline Vector<T, N>& Vector<T, N>::operator*=(T rhs)
{
	for (unsigned int i = 0; i < N; i++)
	{
		elements[i] *= rhs;
	}
	return *this;
}

template<typename T, unsigned int N>
inline Vector<T, N> Vector<T, N>::operator*(T rhs) const
{
	Vector<T, N> result(*this);
	return result *= rhs;
}

template<typename T, unsigned int N>
inline Vector<T, N>& Vector<T, N>::operator/=(T rhs)
{
	return (*this) *= (1.0f / rhs);
}

template<typename T, unsigned int N>
inline Vector<T, N> Vector<T, N>::operator/(T rhs) const
{
	Vector<T, N> result(*this);
	return result /= rhs;
}

template<typename T, unsigned int N>
inline T& Vector<T, N>::operator[](unsigned int i)
{
	return elements[i];
}

template<typename T, unsigned int N>
inline T Vector<T, N>::operator[](unsigned int i) const
{
	return elements[i];
}

template<typename T, unsigned int N>
inline void Vector<T, N>::normalize()
{
	(*this) /= this->length();
}

template<typename T, unsigned int N>
inline T Vector<T, N>::length() const
{
	return std::sqrtf(this->dot(*this));
}

template<typename T, unsigned int N>
inline T Vector<T, N>::dot(const Vector<T, N> & rhs) const
{
	T result = 0;
	for (unsigned int i = 0; i < N; i++)
	{
		result += this->elements[i] * rhs.elements[i];
	}

	return result;
}

template<typename T, unsigned int N>
inline std::ostream& operator<<(std::ostream & out, const Vector<T, N> & v) {
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

template<typename T, unsigned int N>
inline std::istream& operator>>(std::istream & in, Vector<T, N> & v) {
	for (unsigned int i = 0; i < N; i++)
	{
		in >> v[i];
	}
	return in;
}

template<typename T, unsigned int N>
inline Vector<T, N> operator*(T lhs, const Vector<T, N> & rhs) {
	Vector<T, N> result(*this);
	return result *= rhs;
}

template<typename T, unsigned int N>
inline Vector<T, N> operator/(T lhs, const Vector<T, N> & rhs) {
	Vector<T, N> result(*this);
	return result /= rhs;
}
