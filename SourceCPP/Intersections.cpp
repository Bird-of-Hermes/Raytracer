#include "..\SourceHeaders\Intersections.h"

const INTERSECTIONS ClosestHit(const Utils::Vector<INTERSECTIONS>& vector)
{
	float positive = 2000.0f;
	INTERSECTIONS f;
	for (uint32_t i = 0; i < vector.size(); i++)
	{
		if (vector[i].m_t > ERRORMARGIN && vector[i].m_t < positive)
		{
			positive = vector[i].m_t;
			f = vector[i];
		}
	}
	return f;
}
const INTERSECTIONS ClosestHit(Utils::Static_Array<INTERSECTIONS, 12> vector)
{
	float positive = 2000.0f;
	INTERSECTIONS f;
	for (uint32_t i = 0; i < vector.size(); i++)
	{
		if (vector[i].m_t > ERRORMARGIN && vector[i].m_t < positive)
		{
			positive = vector[i].m_t;
			f = vector[i];
		}
	}
	return f;
}
const Color ColorAt(World* world, const Ray& ray)
{
	const INTERSECTIONS f = ClosestHit(IntersectWorld(world, ray));

	if (f.m_t > 0)
		return Shade_Hit(*world, { ray, f});
	else
	{
		return BLACK;
	}
}
const Color HeapColorAt(World* world, const Ray& ray)
{
	INTERSECTIONS* f;
	Mem.Next(&f);
		
	*f = ClosestHit(HeapIntersectWorld(world, ray));

	if (f->m_t > 0)
		return HeapShade_Hit(*world, { ray, *f });
	else
	{
		return BLACK;
	}
}
bool IsShadow(World* world, Tuple::Pos point)
{
	const Tuple::Pos v = world->GetLight().GetPosition() - point;
	const float distance = Tuple::Magnitude(v);
	//const Tuple::Pos direction = Tuple::Normalize(v);
	//const Ray r{ point, Tuple::Normalize(v) };

	const INTERSECTIONS f = ClosestHit(IntersectWorld(world, { point, Tuple::Normalize(v) }));

	if (f.m_t > 0 && f.m_t < distance)
		return true;
	else
		return false;
}
bool HeapIsShadow(World* world, Tuple::Pos point)
{
	const Tuple::Pos v = world->GetLight().GetPosition() - point;
	const float distance = Tuple::Magnitude(v);
	//const Tuple::Pos direction = Tuple::Normalize(v);
	//const Ray r{ point, Tuple::Normalize(v) };

	INTERSECTIONS* f;
	Mem.Next(&f);
	*f = ClosestHit(HeapIntersectWorld(world, { point, Tuple::Normalize(v) }));

	if (f->m_t > 0 && f->m_t < distance)
		return true;
	else
		return false;
}

// Exclusive to chapter 5
const void Intersect(Object* obj, const Ray& R, INTERSECTIONS vector[])
{
	static uint32_t z = 0;

	if (!static_cast<Sphere*>(obj)->GetTransform().isEqual(IdentityMat4x4f()))
	{
		const auto inv = obj->GetTransform().Invert();
		const Ray InverseRay = { inv * R.GetOrigin(), inv * R.GetDirection() };
		std::cout << InverseRay.GetOrigin() << std::endl;
		std::cout << InverseRay.GetDirection() << std::endl;

		const auto SphereToRay = InverseRay.GetOrigin() - Tuple::Point(0.0f, 0.0f, 0.0f);
		const float a = Tuple::DotProduct(InverseRay.GetDirection(), InverseRay.GetDirection());
		const float b = 2.0f * Tuple::DotProduct(InverseRay.GetDirection(), SphereToRay);

		// b²-4ac
		const float discriminant = b * b - 4.0f * a * (Tuple::DotProduct(SphereToRay, SphereToRay) - 1.0f);

		// se não houver intesecções entre o raio e a esfera
		if (discriminant < 0)
		{
			vector[z++] = { 0.0f, *obj };
			return;
		}

		const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
		const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

		if (i1 > 0 && i2 > 0)
		{
			vector[z++] = { std::min(i1,i2), *obj };
		}
		else if (i1 > 0 || i2 > 0)
		{
			vector[z++] = { std::max(i1,i2), *obj };
		}
		else
			vector[z++] = { 0.0f, *obj };
	}
	else
	{
		const auto SphereToRay = R.GetOrigin() - Tuple::Point(0.0f, 0.0f, 0.0f);
		const float a = Tuple::DotProduct(R.GetDirection(), R.GetDirection());
		const float b = 2.0f * Tuple::DotProduct(R.GetDirection(), SphereToRay);

		// b²-4ac
		const float discriminant = b * b - 4.0f * a * (Tuple::DotProduct(SphereToRay, SphereToRay) - 1.0f);

		if (discriminant < 0)
		{
			vector[z++] = { 0.0f, *obj };
			return;
		}

		const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
		const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

		if (i1 > 0 && i2 > 0)
		{
			vector[z++] = { std::min(i1,i2), *obj };
		}
		else if (i1 > 0 || i2 > 0)
		{
			vector[z++] = { std::max(i1,i2), *obj };
		}
		else
			vector[z++] = { 0.0f, *obj };
	}
}