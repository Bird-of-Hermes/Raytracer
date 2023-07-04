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
		os << "Object ID = " << inters.m_obj->GetId() << ",\t";
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
		normalv = NormalAt(intersection.m_obj, point);
		const float z = Tuple::DotProduct(normalv, eyev);
		if (z < 0)
		{
			inside = true;
			normalv = -normalv;
		}
		else
		{
			inside = false;
		}
		over_point = point + normalv * ERRORMARGIN;
	}

public:
	INTERSECTIONS inter;
	Tuple::Pos point;
	Tuple::Pos eyev;
	Tuple::Pos normalv;
	Tuple::Pos over_point;
	bool inside;
};

inline const Color Lighting(const PRECOMPUTATIONS& pc, Light light, bool is_shadow)
{
	Color diffuse, specular;
	Color effective_color;

	if (pc.inter.m_obj->GetMaterial()->m_haspattern == true)
	{
		const Tuple::Pos objpoint {pc.inter.m_obj->GetInvTransform() * pc.point};
		const Tuple::Pos patpoint { pc.inter.m_obj->GetMaterial()->m_pattern.GetInvTransform() * objpoint};
		switch (pc.inter.m_obj->GetMaterial()->m_pattern.GetPattern())
		{
			case PATTERNTYPE::STRIPES: effective_color = StripeAt(pc.inter.m_obj->GetMaterial()->m_pattern, patpoint); break;
			case PATTERNTYPE::CHECKER2D: effective_color = Checker2DAt(pc.inter.m_obj->GetMaterial()->m_pattern, patpoint); break;
			case PATTERNTYPE::CHECKER3D: effective_color = Checker3DAt(pc.inter.m_obj->GetMaterial()->m_pattern, patpoint); break;
			case PATTERNTYPE::INTERPOLATE: effective_color = GradientAt(pc.inter.m_obj->GetMaterial()->m_pattern, patpoint); break;
			case PATTERNTYPE::RING: effective_color = RingAt(pc.inter.m_obj->GetMaterial()->m_pattern, patpoint); break;
			default: break;
		}
	}
	else
	{
		effective_color = pc.inter.m_obj->GetMaterial()->m_Color * light.GetIntensity();
	}
	const Color ambient = effective_color * pc.inter.m_obj->GetMaterial()->m_Ambient;
	if (is_shadow)
	{
		return ambient;
	}
	const Tuple::Pos lightv = Normalize(light.GetPosition() - pc.point);
	const float light_dot_normal = Tuple::DotProduct(lightv, pc.normalv);

	if (light_dot_normal >= 0)
	{
		diffuse = effective_color * pc.inter.m_obj->GetMaterial()->m_Diffuse * light_dot_normal;
		const Tuple::Pos reflectv = Tuple::Reflect(-lightv, pc.normalv);
		const float reflect_dot_eye = Tuple::DotProduct(reflectv, pc.eyev);

		if (reflect_dot_eye > 0)
		{
			const float factor = std::powf(reflect_dot_eye, pc.inter.m_obj->GetMaterial()->m_Shininess);
			specular = light.GetIntensity() * pc.inter.m_obj->GetMaterial()->m_Specular * factor;
		}
	}
	return ambient + diffuse + specular;
}
template<typename T>
const INTERSECTIONS ClosestHit(T& vector, const uint32_t size);
const void Intersect(Object* obj, const Ray& R, INTERSECTIONS vector[]);
template <uint32_t Size>
const void FullIntersection(Object* obj, const Ray& R, Utils::Static_Array<INTERSECTIONS, Size>& vector);
const void FullIntersection(Object* obj, const Ray& R, Utils::Vector<INTERSECTIONS>& vector);
template <uint32_t Size>
inline const void IntersectWorld(World* world, const Ray& R, Utils::Static_Array<INTERSECTIONS, Size>& vector)
{
	for (uint32_t i = 0; i < world->getObjVector().size(); i++)
	{
		FullIntersection(world->getObjVector().operator[](i), R, vector);
	}
}
inline const void IntersectWorld(World* world, const Ray& R, Utils::Vector<INTERSECTIONS>& vector)
{
	for (uint32_t i = 0; i < world->getObjVector().size(); i++)
	{
		FullIntersection(world->getObjVector().operator[](i), R, vector);
	}
}
inline const void SortIntersections(std::vector<INTERSECTIONS>& vector) { std::sort(vector.begin(), vector.end()); }
bool IsShadow(World* world, Tuple::Pos point);
inline const Color Shade_Hit(World& world, const PRECOMPUTATIONS& pc)
{
	return Lighting(pc, world.GetLight(), IsShadow(&world, pc.over_point));
}
const Color ColorAt(World* world, const Ray& ray, Utils::Static_Array<INTERSECTIONS, 12>& vector);
const Color ColorAt(World* world, const Ray& ray, Utils::Vector<INTERSECTIONS>& vector);
#endif