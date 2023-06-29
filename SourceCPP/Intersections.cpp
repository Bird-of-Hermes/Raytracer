#include "..\SourceHeaders\Intersections.h"

const INTERSECTIONS ClosestHit(Utils::Vector<INTERSECTIONS>& vector, const uint32_t size)
{
	static uint32_t counter = 1;
	uint32_t num = (counter - 1) * (size * 2);
	float maiorquezero = 10000000.0f;
	INTERSECTIONS f = vector[0];
	for (uint32_t i = num; i < num + (size * 2); i++)
	{
		if (vector[i].m_t > 0.0f && vector[i].m_t < maiorquezero)
		{
			maiorquezero = vector[i].m_t;
			f = vector[i];
		}
	}
	++counter;
	return f;
}

const void Intersect(Object* obj, const Ray& R, INTERSECTIONS vector[])
{
	static uint32_t z = 0;

	if (!static_cast<Sphere*>(obj)->GetTransform().isEqual(IdentityMat4x4f()))
	{
		const auto inv = static_cast<Sphere*>(obj)->GetTransform().Invert();
		const Ray InverseRay = { inv * R.GetOrigin(), inv * R.GetDirection() };

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

const void FullIntersection(Object* obj, const Ray& R, Utils::Vector<INTERSECTIONS>& vector)
{
	if (!static_cast<Sphere*>(obj)->GetTransform().isEqual(IdentityMat4x4f()))
	{
		const Matrix4x4f inv = static_cast<Sphere*>(obj)->GetTransform().Invert();
		const Ray InverseRay = { inv * R.GetOrigin(), inv * R.GetDirection() };

		const Tuple::Pos SphereToRay = InverseRay.GetOrigin() - Tuple::Point(0.0f, 0.0f, 0.0f);
		const float a = Tuple::DotProduct(InverseRay.GetDirection(), InverseRay.GetDirection());
		const float b = 2.0f * Tuple::DotProduct(InverseRay.GetDirection(), SphereToRay);

		// b²-4ac
		const float discriminant = b * b - 4.0f * a * (Tuple::DotProduct(SphereToRay, SphereToRay) - 1.0f);

		// se não houver intersecções entre o raio e a esfera
		if (discriminant < 0)
		{
			vector.fill_in({ 0.0f, *obj });
			vector.fill_in({ 0.0f, *obj });
			return;
		}

		const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
		const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

		vector.fill_in({ i1, *obj });
		vector.fill_in({ i2, *obj });
	}
	else
	{
		const Tuple::Pos SphereToRay = R.GetOrigin() - Tuple::Point(0.0f, 0.0f, 0.0f);
		const float a = Tuple::DotProduct(R.GetDirection(), R.GetDirection());
		const float b = 2.0f * Tuple::DotProduct(R.GetDirection(), SphereToRay);

		// b²-4ac
		const float discriminant = b * b - 4.0f * a * (Tuple::DotProduct(SphereToRay, SphereToRay) - 1.0f);

		if (discriminant < 0)
		{
			vector.fill_in({ 0.0f, *obj });
			vector.fill_in({ 0.0f, *obj });
			return;
		}

		const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
		const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

		vector.fill_in({ i1, *obj });
		vector.fill_in({ i2, *obj });
	}
}

Color ColorAt(World* world, const Ray& ray, Utils::Vector<INTERSECTIONS>& vector)
{
	const size_t md = vector.Size();
	IntersectWorld(world, ray, vector);

#if true
	const INTERSECTIONS f = ClosestHit(vector, static_cast<uint32_t>(world->getObjVector().size()));

	if (f.m_t > 0)
		return Shade_Hit(*world, { ray, f });
	else
		return BLACK;
#endif

#if false

	static size_t abc = 1;
	//const size_t loopnumber = vector.Size() / world->getObjVector().size();
	std::sort(&vector[0] + (abc - 1) * world->getObjVector().size(), &vector[0] + vector.Size());
	
	// If Hit()
	for (size_t i = (abc-1) * world->getObjVector().size(); i < vector.Size(); i++)
	{
		if (vector.at(i)->m_t > 0)
		{
			++abc;
			return Shade_Hit(*world, { ray, vector[i] });
		}
	}
	++abc;
	return GRAY;
	
#endif
}