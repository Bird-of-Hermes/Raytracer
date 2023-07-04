#ifndef PATTERNS_H
#define PATTERNS_H
#include "Tuples.h"
#include "Matrixes.h"

enum PATTERNTYPE { STRIPES = 0, CHECKER2D, CHECKER3D, INTERPOLATE, RING};

class PATTERNS
{
private:
	Color m_a = WHITE;
	Color m_b = BLACK;
	Matrix4x4f m_InvTransform;
	PATTERNTYPE m_pattype;

public:
	PATTERNS(Color a = WHITE, Color b = BLACK, PATTERNTYPE pattype = PATTERNTYPE::STRIPES) : m_a(a), m_b(b), m_InvTransform(IdentityMat4x4f()), m_pattype(pattype){}
	const Color GetColorA() const { return m_a; }
	const Color GetColorB() const { return m_b; }
	const void SetPattern(PATTERNTYPE pattype) { m_pattype = pattype; }
	const uint32_t GetPattern() const { return m_pattype; }
	const void SetTransform(const Matrix4x4f& mat) { m_InvTransform = mat.Invert(); }
	const Matrix4x4f GetInvTransform() const { return m_InvTransform; }
};

inline const Color StripeAt(const PATTERNS& pat, Tuple::Pos worldpoint)
{
	if (static_cast<int>(worldpoint.m_x) % 2 == 0)
	{
		return pat.GetColorA();
	}
	else
		return pat.GetColorB();
}
inline const Color GradientAt(const PATTERNS& gradient, Tuple::Pos worldpoint)
{
	return gradient.GetColorA() + (gradient.GetColorB() - gradient.GetColorA()) * (worldpoint.m_x - floorf(worldpoint.m_x));
}
inline const Color RingAt(const PATTERNS& pattern, Tuple::Pos worldpoint)
{
	if (static_cast<int>(std::sqrtf(worldpoint.m_x * worldpoint.m_x + worldpoint.m_z * worldpoint.m_z)) % 2 == 0) // same as flooring
	{
		return pattern.GetColorA();
	}
	else
		return pattern.GetColorB();
}
inline const Color Checker2DAt(const PATTERNS& pattern, Tuple::Pos worldpoint)
{
	if (static_cast<uint32_t>(std::floor(worldpoint.m_x) + std::floor(worldpoint.m_z)) % 2 == 0) // same as flooring 
	{
		return pattern.GetColorA();
	}
	else
	{
		return pattern.GetColorB();
	}
}
inline const Color Checker3DAt(const PATTERNS& pattern, Tuple::Pos worldpoint)
{
	if (static_cast<uint32_t>(std::floor(worldpoint.m_x) + std::floor(worldpoint.m_y) + std::floor(worldpoint.m_z)) % 2 == 0) // same as flooring 
	{
		return pattern.GetColorA();
	}
	else
	{
		return pattern.GetColorB();
	}
}
#endif