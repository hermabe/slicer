#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include <array>

constexpr float PI = 3.14159265359f;
constexpr float FLOATERROR = 1e-5f;

template<typename T, unsigned int N>
class Vector
{
public:
	template<typename... Args>
	Vector<T, N>(Args... args) : elements{ args... } {
		static_assert(std::is_arithmetic<T>::value, "Type must be numeric");
		static_assert(sizeof... (Args) == N, "Wrong number of elements supplied in constructor");
	}
	Vector<T, N>() = default;
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
	Vector<T, N> normalized() const;
	T length() const;
	T dot(const Vector<T, N>& rhs) const;
	bool isClose(const Vector<T, N>& rhs, T tolerance = FLOATERROR) const;
private:
	T elements[N]{};
};

typedef Vector<float, 3> Vector3d;
typedef Vector<float, 2> Vector2d;

Vector3d cross(const Vector3d& lhs, const Vector3d& rhs);

template<typename T, unsigned int N>
class Triangle
{
public:
	Vector<T, N> normal;
	std::array<Vector<T, N>, 3> vertices;
};

typedef Triangle<float, 3> Triangle3d;
typedef Triangle<float, 2> Triangle2d;

template<typename T, unsigned int N>
struct Edge {
	Vector<T, N> start, end, normal;
};

typedef Edge<float, 3> Edge3d;
typedef Edge<float, 2> Edge2d;

class Plane {
public:
	Vector3d point, normal, xAxis;

	Plane(const Vector3d& point, const Vector3d& normal, const Vector3d& xAxis) : point(point), normal(normal.normalized()), xAxis(xAxis.normalized()) {};
	Vector3d yAxis() const {
		return cross(normal, xAxis).normalized();
	}
};

class Line {
public:
	Vector3d point, direction;

	Line(const Edge3d& edge) : point(edge.start), direction(edge.end - edge.start) {};
	Line(const Vector3d& point, const Vector3d& direction) : point(point), direction(direction) {};
};

struct SimplePolygon {
	// Represents a simply connected polygon
	std::vector<Vector2d> vertices;
};

struct Polygon {
	// Represents a complex polygon.
	// Can have zero or more "holes" which are SimplePolygons
	std::vector<SimplePolygon> paths;
};

std::vector<Triangle3d> intersects(const std::vector<Triangle3d>& triangles, const Plane& plane);
Vector3d intersection(const Plane& plane, const Line& line);
std::vector<Edge3d> intersection(const Plane& plane, const Triangle3d& triangle);

Vector2d project(const Plane& plane, const Vector3d& vector);
Edge2d project(const Plane& plane, const Edge3d& edge);

std::vector<Triangle3d> withVertex(const std::vector<Triangle3d>& triangles, const Vector3d& vertex);
Triangle3d withEdge(const std::vector<Triangle3d>& triangles, const Vector3d& firstVertex, const Vector3d& secondVertex);
Triangle3d withEdge(const std::vector<Triangle3d>& withVertex, const Vector3d& secondVertex);

float directionalAngle(const Vector2d& lhs, const Vector2d& rhs);
float saturate(float value, float minimum, float maximum);

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
inline Vector<T, N> Vector<T, N>::normalized() const
{
	return *this / length();
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
inline bool Vector<T, N>::isClose(const Vector<T, N> & rhs, T tolerance) const
{
	return (*this - rhs).length() < tolerance;
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
