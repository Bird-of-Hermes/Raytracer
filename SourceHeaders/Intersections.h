#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include <algorithm>
#include <vector>
#include "Ray.h"
#include "Tuples.h"
#include "Matrixes.h"
#include "Objects.h"
#include "Utils.h"

// STRUCTS
struct INTERSECTIONS
{
	INTERSECTIONS() = default;
	INTERSECTIONS(float point, Object& object) : m_t(point), m_obj(&object) {}
	const bool operator<(const INTERSECTIONS& other) const
	{
		return m_t < other.m_t;
	}
	friend std::ostream& operator<<(std::ostream& os, const INTERSECTIONS& inters)
	{
		os << "Sphere ID = " << inters.m_obj->GetId() << ",\t";
		os << "intersected at t = " << inters.m_t << '.' << std::endl;

		return os;
	}

public:
	Object* m_obj;
	float m_t;
};
struct PRECOMPUTATIONS
{
	PRECOMPUTATIONS() = default;
	PRECOMPUTATIONS(const Ray& ray, const INTERSECTIONS& intersection) :	
		inter(intersection), 
		point(ray.Position(intersection.m_t)), 
		eyev(-ray.GetDirection()) 
	{
		normalv = static_cast<Sphere*>(intersection.m_obj)->NormalAt(point);
		const auto z = Tuple::DotProduct(normalv, eyev);
		if (z < 0)
		{
			inside = true;
			normalv = -normalv;
		}
		else
		{
			inside = false;
		}
	}

public:
	INTERSECTIONS inter;
	Tuple::Pos point;
	Tuple::Pos eyev;
	Tuple::Pos normalv;
	bool inside;
};


const INTERSECTIONS ClosestHit(Utils::Vector<INTERSECTIONS>& vector, const size_t size);
const void Intersect(Object* obj, const Ray& R, INTERSECTIONS vector[]);
const void FullIntersection(Object* obj, const Ray& R,Utils::Vector<INTERSECTIONS>& vector);
inline const void IntersectWorld(World* world, const Ray& R, Utils::Vector<INTERSECTIONS>& vector)
{
	for (size_t i = 0; i < world->getObjVector().size(); i++)
	{
		FullIntersection(world->getObjVector().operator[](i), R, vector);
	}
}
inline const void SortIntersections(std::vector<INTERSECTIONS>& vector) { std::sort(vector.begin(), vector.end()); }
inline const Color Shade_Hit(World& world, const PRECOMPUTATIONS& pc)
{
	return Lighting(static_cast<Sphere*>(pc.inter.m_obj)->GetMaterial(), world.GetLight(), pc.point, pc.eyev, pc.normalv, false);
}
Color ColorAt(World* world, const Ray& ray, Utils::Vector<INTERSECTIONS>& vector);
#endif