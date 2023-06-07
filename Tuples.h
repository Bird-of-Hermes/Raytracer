#ifndef TUPLES_H
#define TUPLES_H
#define ERRORMARGIN 0.001f

#include <iostream>

namespace Tuple
{
	struct Pos
	{
		float m_x;
		float m_y;
		float m_z;
		int32_t m_type;

		Pos(float x = 0.0f, float y = 0.0f, float z = 0.0f, int32_t type = 0) : m_x(x), m_y(y), m_z(z), m_type(type) {} // true (1) for points, false (0) for vectors
		Pos operator+(const Pos& b) { return { this->m_x + b.m_x, this->m_y + b.m_y, this->m_z + b.m_z, abs(this->m_type + b.m_type) }; }
		Pos operator-(const Pos& b) { return { this->m_x - b.m_x, this->m_y - b.m_y, this->m_z - b.m_z, abs(this->m_type - b.m_type) }; }
		Pos operator-() const		{ return { -m_x, -m_y, -m_z }; }
		Pos operator*(const Pos& b) { return { this->m_x * b.m_x, this->m_y * b.m_y, this->m_z * b.m_z, abs(this->m_type * b.m_type) }; }
		Pos operator*(float b)	const	{ return { this->m_x * b, this->m_y * b, this->m_z * b }; }
		friend std::ostream& operator<<(std::ostream& os, const Pos& pos) {os << "Tuple::Pos (" << pos.m_x << ", " << pos.m_y << ", " << pos.m_z << ", " << pos.m_type << ")"; return os;}
	};

	inline const Pos Vector(float x, float y, float z) { return Pos{ x,y,z, 0 }; }
	inline const Pos Vector(Pos xyz) { return { xyz.m_x, xyz.m_y, xyz.m_z, 0 }; }
	inline const Pos Point(float x, float y, float z) { return Pos{ x,y,z, 1 }; }
	inline const Pos Point(Pos xyz) { return { xyz.m_x, xyz.m_y, xyz.m_z, 1 }; }
	inline const int32_t Equals(float a, float b) { if (abs(a - b) < ERRORMARGIN) return true; else return false; }
	inline const float FastSqrt(const float x)
	{
		union
		{
			float x;
			int i;
		} u{};
		u.x = x;
		u.i = (1 << 29) + (u.i >> 1) - (1 << 22);  // Valor inicial estimado
		return u.x;
	}
	inline const float Magnitude(const Pos& a) { return FastSqrt(a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z); }
	inline const Pos Normalize(const Pos& a) { float magnitude = Magnitude(a); return { a.m_x / magnitude, a.m_y / magnitude, a.m_z / magnitude }; }
	// the m_type is unnecessary but might help spotting bugs
	inline const float DotProduct(const Pos& a, const Pos&b) { return { a.m_x * b.m_x + a.m_y * b.m_x + a.m_z * b.m_z + a.m_type* b.m_type}; }
	inline const Pos CrossProduct(const Pos& a, const Pos& b) { return { a.m_y * b.m_z - a.m_z * b.m_y, a.m_z * b.m_x - a.m_x * b.m_z, a.m_x * b.m_y - a.m_y * b.m_x }; }
}

struct Color
{
	float r;
	float g;
	float b;

	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f) : r(red), g(green), b(blue) {}
	Color(Tuple::Pos rgb) { r = rgb.m_x; g = rgb.m_y; b = rgb.m_z; }

	Color operator+(const Color& b) { return { (r + b.r)/2.0f, (g + b.g)/2.0f, (this->b + b.b)/2.0f }; }
	Color operator-(const Color& b) { return { abs(r - b.r), abs(g - b.g), abs(this->b - b.b) }; }
	Color operator*(const Color& b) { return { this->r * b.r, this->g * b.g, this->b * b.b }; }
	Color operator*(float b) { return { r * b, g * b, this->b * b }; }
	friend std::ostream& operator<<(std::ostream& os, const Color& cor) { os << "RGB (" << cor.r << ", " << cor.g << ", " << cor.b << ")"; return os; }
};

#endif