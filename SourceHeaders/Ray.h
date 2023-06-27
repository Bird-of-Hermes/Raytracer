#ifndef RAY_H
#define RAY_H
#include "Tuples.h"
#include "Matrixes.h"

class Ray
{
public:
	Ray(Tuple::Pos genesis, Tuple::Pos dir) : origin(genesis), direction(dir){}
	const Tuple::Pos Position(float t) const { return origin + (direction * t); }
	const Tuple::Pos GetOrigin() const { return origin; }
	const Tuple::Pos GetDirection() const { return direction; }

private:
	Tuple::Pos origin; // Tuple::Pos Point
	Tuple::Pos direction; // Tuple::Pos Vector
};

inline const Ray Transform(const Ray& r, const Matrix4x4f& Translation)
{
	return { Translation * r.GetOrigin(), Translation* r.GetDirection() };
}

#endif