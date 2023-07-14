#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include <algorithm>
#include <memory>
#include <vector>
#include "Ray.h"
#include "Tuples.h"
#include "Matrixes.h"
#include "Objects.h"
#include "Utils.h"

// STRUCTS
struct INTERSECTIONS
{
	INTERSECTIONS() : m_t(NULL), m_obj(nullptr) {}
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
	float m_t;
	Object* m_obj;
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
		reflectv = Tuple::Reflect(ray.GetDirection(), normalv);
	}
	PRECOMPUTATIONS(const Ray& ray, const INTERSECTIONS& intersection, const Utils::Static_Array<INTERSECTIONS, 12>& xs) :
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
		reflectv = Tuple::Reflect(ray.GetDirection(), normalv);
		/////////////////////////////////////////////
		//for (size_t i = 0; i < xs.size(); i++)
		//{/*
		//	if (xs[i])
		//	{

		//	}*/
		//}

	}
public:
	INTERSECTIONS inter;
	Tuple::Pos point;
	Tuple::Pos eyev;
	Tuple::Pos normalv;
	Tuple::Pos over_point;
	Tuple::Pos reflectv;
	bool inside;
};

const Color HeapColorAt(World* world, const Ray& ray);
const Color ColorAt(World* world, const Ray& ray);

inline const Color Lighting(const PRECOMPUTATIONS& pc, const Light& light, bool is_shadow)
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
		const float reflect_dot_eye = Tuple::DotProduct(pc.reflectv, pc.eyev);

		if (reflect_dot_eye > 0)
		{
			//const float factor = std::powf(reflect_dot_eye, pc.inter.m_obj->GetMaterial()->m_Shininess);
			specular = light.GetIntensity() * pc.inter.m_obj->GetMaterial()->m_Specular * std::powf(reflect_dot_eye, pc.inter.m_obj->GetMaterial()->m_Shininess);
		}
	}
	return ambient + diffuse + specular;
}

const INTERSECTIONS ClosestHit(const Utils::Vector<INTERSECTIONS>& vector);
const INTERSECTIONS ClosestHit(Utils::Static_Array<INTERSECTIONS,12> vector);

bool IsShadow(World* world, Tuple::Pos point);
bool HeapIsShadow(World* world, Tuple::Pos point);

static thread_local uint32_t recursion = 0;
inline const Color ReflectedColor(World* world, const PRECOMPUTATIONS& comps)
{
	if (comps.inter.m_obj->GetMaterial()->m_Reflectiveness == 0)
	{
		return BLACK;
	}
	if (recursion < 1024) // 256 = 16 samples ~ 1024 = 32 samples
	{
		++recursion;
	}
	else
	{
		recursion = 0;
		return BLACK;
	}
	return ColorAt(world, { comps.over_point, comps.reflectv }) * comps.inter.m_obj->GetMaterial()->m_Reflectiveness;
	// Ray reflectedRay{comps.over_point, comps.reflectv};
	// Color colorat{ ColorAt(world, {comps.over_point, comps.reflectv}, array) };
	// return colorat * m_Reflectiveness;
}
inline const Color HeapReflectedColor(World* world, const PRECOMPUTATIONS& comps)
{
	if (comps.inter.m_obj->GetMaterial()->m_Reflectiveness == 0)
	{
		return BLACK;
	}
	if (recursion < 2768) // 256 = 16 samples ~ 1024 = 32 samples
	{
		++recursion;
	}
	else
	{
		recursion = 0;
		return BLACK;
	}
	return HeapColorAt(world, { comps.over_point, comps.reflectv }) * comps.inter.m_obj->GetMaterial()->m_Reflectiveness;
	// Ray reflectedRay{comps.over_point, comps.reflectv};
	// Color colorat{ ColorAt(world, {comps.over_point, comps.reflectv}, array) };
	// return colorat * m_Reflectiveness;
}

inline const Color Shade_Hit(World& world, const PRECOMPUTATIONS& pc)
{
	return ReflectedColor(&world, pc) + Lighting(pc, world.GetLight(), IsShadow(&world, pc.over_point));
}
inline const Color HeapShade_Hit(World& world, const PRECOMPUTATIONS& pc)
{
	return HeapReflectedColor(&world, pc) + Lighting(pc, world.GetLight(), IsShadow(&world, pc.over_point));
}

inline const Utils::Static_Array<INTERSECTIONS, 12> IntersectWorld(World* world, const Ray& R)
{
	Utils::Static_Array<INTERSECTIONS, 12> vector;
	for (uint32_t i = 0; i < world->getObjVector().size(); i++)
	{
		if (!world->getObjVector()[i]->GetTransform().isEqual(IdentityMat4x4f()))
		{
			const Ray InverseRay = { world->getObjVector()[i]->GetInvTransform() * R.GetOrigin(), world->getObjVector()[i]->GetInvTransform() * R.GetDirection() };

			if (world->getObjVector()[i]->GetType() == OBJTYPE::PLANE)
			{
				if (fabsf(InverseRay.GetDirection().m_y) < ERRORMARGIN)
					continue;

				vector.fill_in({ -InverseRay.GetOrigin().m_y / InverseRay.GetDirection().m_y, *world->getObjVector()[i] });
					continue;
			}

			const Tuple::Pos SphereToRay = InverseRay.GetOrigin() - Tuple::Point(0.0f, 0.0f, 0.0f);
			const float a = Tuple::DotProduct(InverseRay.GetDirection(), InverseRay.GetDirection());
			const float b = 2.0f * Tuple::DotProduct(InverseRay.GetDirection(), SphereToRay);

			// b²-4ac
			const float discriminant = b * b - 4.0f * a * (Tuple::DotProduct(SphereToRay, SphereToRay) - 1.0f);

			// if it doesn't intersect
			if (discriminant < 0)
				continue;

			const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
			const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

			vector.fill_in({ i1, *world->getObjVector()[i] });
			vector.fill_in({ i2, *world->getObjVector()[i] });
		}
		else
		{
			if (world->getObjVector()[i]->GetType() == OBJTYPE::PLANE)
			{
				if (fabsf(R.GetDirection().m_y) < ERRORMARGIN)
					continue;

				vector.fill_in({ -R.GetOrigin().m_y / R.GetDirection().m_y, *world->getObjVector()[i] });
					continue;
			}

			const Tuple::Pos SphereToRay = R.GetOrigin() - Tuple::Point(0.0f, 0.0f, 0.0f);
			const float a = Tuple::DotProduct(R.GetDirection(), R.GetDirection());
			const float b = 2.0f * Tuple::DotProduct(R.GetDirection(), SphereToRay);

			// b²-4ac
			const float discriminant = b * b - 4.0f * a * (Tuple::DotProduct(SphereToRay, SphereToRay) - 1.0f);

			// if it doesn't intersect
			if (discriminant < 0)
				continue;

			const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
			const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

			vector.fill_in({ i1, *world->getObjVector()[i] });
			vector.fill_in({ i2, *world->getObjVector()[i] });
		}
	}
	return vector;
}
inline const Utils::Vector<INTERSECTIONS> HeapIntersectWorld(World* world, const Ray& R)
{
	Utils::Vector<INTERSECTIONS>* vector;
	Mem.Next(&vector);
	for (uint32_t i = 0; i < world->getObjVector().size(); i++)
	{
		if (!world->getObjVector()[i]->GetTransform().isEqual(IdentityMat4x4f()))
		{
			Ray* InverseRay;
			Mem.Next(&InverseRay);
			*InverseRay = { world->getObjVector()[i]->GetInvTransform() * R.GetOrigin(), world->getObjVector()[i]->GetInvTransform() * R.GetDirection() };

			if (world->getObjVector()[i]->GetType() == OBJTYPE::PLANE)
			{
				if (fabsf(InverseRay->GetDirection().m_y) < ERRORMARGIN)
					continue;

				vector->emplace_back({ -InverseRay->GetOrigin().m_y / InverseRay->GetDirection().m_y, *world->getObjVector()[i] });
				continue;
			}

			const Tuple::Pos SphereToRay = InverseRay->GetOrigin() - Tuple::Point(0.0f, 0.0f, 0.0f);
			const float a = Tuple::DotProduct(InverseRay->GetDirection(), InverseRay->GetDirection());
			const float b = 2.0f * Tuple::DotProduct(InverseRay->GetDirection(), SphereToRay);

			// b²-4ac
			const float discriminant = b * b - 4.0f * a * (Tuple::DotProduct(SphereToRay, SphereToRay) - 1.0f);

			// if it doesn't intersect
			if (discriminant < 0)
				continue;

			const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
			const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

			vector->emplace_back({ i1, *world->getObjVector()[i] });
			vector->emplace_back({ i2, *world->getObjVector()[i] });
		}
		else
		{
			if (world->getObjVector()[i]->GetType() == OBJTYPE::PLANE)
			{
				if (fabsf(R.GetDirection().m_y) < ERRORMARGIN)
					continue;

				vector->emplace_back({ -R.GetOrigin().m_y / R.GetDirection().m_y, *world->getObjVector()[i] });
				continue;
			}

			const Tuple::Pos SphereToRay = R.GetOrigin() - Tuple::Point(0.0f, 0.0f, 0.0f);
			const float a = Tuple::DotProduct(R.GetDirection(), R.GetDirection());
			const float b = 2.0f * Tuple::DotProduct(R.GetDirection(), SphereToRay);

			// b²-4ac
			const float discriminant = b * b - 4.0f * a * (Tuple::DotProduct(SphereToRay, SphereToRay) - 1.0f);

			// if it doesn't intersect
			if (discriminant < 0)
				continue;

			const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
			const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

			vector->emplace_back({ i1, *world->getObjVector()[i] });
			vector->emplace_back({ i2, *world->getObjVector()[i] });
		}
	}
	return *vector;
}

//Exclusive to chapter 5
const void Intersect(Object* obj, const Ray& R, INTERSECTIONS vector[]);
#endif