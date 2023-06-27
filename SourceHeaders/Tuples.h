#ifndef TUPLES_H
#define TUPLES_H
#define ERRORMARGIN 0.001f

#include <iostream>
#include <math.h>
inline const float FastSqrt(const float x)
{
	union
	{
		float x;
		int i;
	} u{};
	u.x = x;
	u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
	
	const float xhalf = 0.5f * u.x;
	u.x = u.x * (1.5f - xhalf * u.x * u.x); // Newton-Raphson 1st iteration
	u.x = u.x * (1.5f - xhalf * u.x * u.x); // Newton-Raphson 2nd iteration

	return u.x;
}

namespace Tuple
{
	struct Pos
	{
		float m_x;
		float m_y;
		float m_z;
		float m_type;

		Pos(float x = 0.0f, float y = 0.0f, float z = 0.0f, float type = 0) : m_x(x), m_y(y), m_z(z), m_type(type) {} // true (1) for points, false (0) for vectors
		Pos operator+(const Pos& b) const { return { this->m_x + b.m_x, this->m_y + b.m_y, this->m_z + b.m_z, abs(this->m_type + b.m_type) }; }
		Pos operator-(const Pos& b) const{ return { this->m_x - b.m_x, this->m_y - b.m_y, this->m_z - b.m_z, abs(this->m_type - b.m_type) }; }
		Pos operator-() const { return { -m_x, -m_y, -m_z }; }
		Pos operator*(const Pos& b) const { return { this->m_x * b.m_x, this->m_y * b.m_y, this->m_z * b.m_z, abs(this->m_type * b.m_type) }; }
		Pos operator*(float b)	const { return { this->m_x * b, this->m_y * b, this->m_z * b }; }
		friend std::ostream& operator<<(std::ostream& os, const Pos& pos) 
		{
			if(pos.m_type == 0)
				os << "Vector (" << pos.m_x << ", " << pos.m_y << ", " << pos.m_z << ")";
			else
				os << "Point (" << pos.m_x << ", " << pos.m_y << ", " << pos.m_z << ")";

			return os;
		}
	};

	inline const Pos Vector(float x, float y, float z) { return Pos{ x,y,z, 0 }; }
	inline const Pos Vector(Pos xyz) { return { xyz.m_x, xyz.m_y, xyz.m_z, 0 }; }
	inline const Pos Point(float x, float y, float z) { return Pos{ x,y,z, 1 }; }
	inline const Pos Point(Pos xyz) { return { xyz.m_x, xyz.m_y, xyz.m_z, 1 }; }
	inline const bool Equals(float a, float b) { if (abs(a - b) < ERRORMARGIN) return true; else return false; }
	inline const float Magnitude(const Pos& a) { return std::sqrtf(a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z); }
	inline const Pos Normalize(const Pos& a) 
	{
		const float invMagnitude = 1.0f / Magnitude(a);
		return { a.m_x * invMagnitude, a.m_y * invMagnitude, a.m_z * invMagnitude };
	}
	inline const float DotProduct(const Pos& a, const Pos&b) { return  a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z + a.m_type * b.m_type; }
	inline const Pos CrossProduct(const Pos& a, const Pos& b) { return { a.m_y * b.m_z - a.m_z * b.m_y, a.m_z * b.m_x - a.m_x * b.m_z, a.m_x * b.m_y - a.m_y * b.m_x }; }
	inline const Pos Reflect(const Pos& in, const Pos& normal) { return in - normal * 2.0f * DotProduct(in, normal); }
}

struct Color
{
	float r;
	float g;
	float b;

	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f) : r(red), g(green), b(blue) {}
	Color(Tuple::Pos rgb) { r = rgb.m_x; g = rgb.m_y; b = rgb.m_z; }
	
	const Color operator+(const Color& other) const { return { (r + other.r) , (g + other.g), (b + other.b) }; }
	const Color operator-(const Color& other) const { return { abs(r - other.r), abs(g - other.g), abs(b - other.b) }; }
	const Color operator*(const Color& other) const { return { r * other.r, g * other.g, b * other.b }; }
	const Color operator*(float other) const { return { r * other, g * other, b * other }; }
	const Color operator/(float other) const { return { r / other, g / other, b / other }; }
	friend std::ostream& operator<<(std::ostream& os, const Color& cor) { os << "RGB (" << cor.r << ", " << cor.g << ", " << cor.b << ")"; return os; }
};
#define RED Color{1.0f,0.0f,0.0f}
#define GREEN Color{0.0f, 1.0f, 0.0f}
#define BLUE Color{0.0f, 0.0f, 1.0f}
#define WHITE Color{1.0f, 1.0f, 1.0f}
#define BLACK Color{0.0f, 0.0f, 0.0f}

#endif