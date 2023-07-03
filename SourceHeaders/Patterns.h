#ifndef PATTERNS_H
#define PATTERNS_H
#include "Tuples.h"
#include "Matrixes.h"

class PATTERNS
{
private:
	Color m_a = WHITE;
	Color m_b = BLACK;
	Matrix4x4f m_transform;

public:
	PATTERNS(Color a = WHITE, Color b = BLACK) : m_a(a), m_b(b), m_transform(IdentityMat4x4f()){}
	const Color StripeAt(Tuple::Pos point) const
	{
		if (static_cast<int>(point.m_x) % 2 == 0)
		{
			return m_a;
		}
		else
			return m_b;
	}
	const Color GetColorA() const { return m_a; }
	const Color GetColorB() const { return m_b; }
	const void SetTransform(const Matrix4x4f& mat) { m_transform = mat; }
	const Matrix4x4f GetTransform() const { return m_transform; }
	
};
#endif
