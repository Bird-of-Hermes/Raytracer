#include "..\SourceHeaders\Intersections.h"

template<typename T>
const INTERSECTIONS ClosestHit(T& vector, const uint32_t size)
{
	float positive = 10000.0f;
	INTERSECTIONS f = vector[0];
	for (uint32_t i = 0; i < vector.size(); i++)
	{
		if (vector[i].m_t > 0.0f && vector[i].m_t < positive)
		{
			positive = vector[i].m_t;
			f = vector[i];
		}
	}
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

		// if it doesn't intersect
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

		// if it doesn't intersect
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
const void FullIntersection(Object* obj, const Ray& R, Utils::Static_Array<INTERSECTIONS, 4>& vector)
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

		// if it doesn't intersect
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

		// if it doesn't intersect
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
const void FullIntersection(Object* obj, const Ray& R, Utils::Static_Array<INTERSECTIONS, 12>& vector)
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

		// if it doesn't intersect
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

		// if it doesn't intersect
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
const Color ColorAt(World* world, const Ray& ray, Utils::Vector<INTERSECTIONS>& vector)
{
	IntersectWorld(world, ray, vector);

	const INTERSECTIONS f = ClosestHit(vector, world->getObjVector().size());

	if (f.m_t > 0)
		return Shade_Hit(*world, { ray, f });
	else
		return GRAY;
}
const Color ColorAt(World* world, const Ray& ray, Utils::Static_Array<INTERSECTIONS, 12>& vector)
{
	IntersectWorld(world, ray, vector);

	const INTERSECTIONS f = ClosestHit(vector, world->getObjVector().size());

	if (f.m_t > 0)
		return Shade_Hit(*world, { ray, f });
	else
		return GRAY;
}
bool IsShadow(World* world, Tuple::Pos point)
{
	const Tuple::Pos v = world->GetLight().GetPosition() - point;
	const float distance = Tuple::Magnitude(v);
	//const Tuple::Pos direction = Tuple::Normalize(v);
	const Ray r{ point, Tuple::Normalize(v) };
	Utils::Static_Array<INTERSECTIONS, 12> sai;
	IntersectWorld(world, r, sai);
	auto f = ClosestHit(sai, world->getObjVector().size());
	if (f.m_t > 0 && f.m_t < distance)
		return true;
	else
		return false;
}