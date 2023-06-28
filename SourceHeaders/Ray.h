#ifndef RAY_H
#define RAY_H
#include "Tuples.h"
#include "Matrixes.h"
#include "Objects.h"

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

inline const Ray RayForPixel(const Camera& cam, uint32_t width, uint32_t height)
{
	const Matrix4x4f caminvert = cam.GetTransform().Invert();
	/*
	const float xoffset = (width + 0.5f) * cam.GetPixelSize();
	const float yoffset = (height + 0.5f) * cam.GetPixelSize();

	const float worldx = cam.GetHalfsArray()[1] - xoffset;
	const float worldy = cam.GetHalfsArray()[2] - yoffset;
	*/

	const Tuple::Pos origin = caminvert * Tuple::Point(0, 0, 0);
	/*
	const auto pixel = caminvert * Tuple::Point(worldx, worldy, -1.0f);
	const auto direction = Tuple::Normalize(pixel - origin);
	return { origin, direction }; */

	return { origin, Tuple::Normalize(caminvert * Tuple::Point(cam.GetHalfsArray()[1] - (width + 0.5f) * cam.GetPixelSize(), cam.GetHalfsArray()[2] - (height + 0.5f) * cam.GetPixelSize(), -1.0f) - origin) };
}

#endif