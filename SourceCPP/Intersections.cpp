#include "..\SourceHeaders\Intersections.h"

const INTERSECTIONS Hit(std::vector<INTERSECTIONS>& vector)
{
	for (size_t i = 0; i < vector.size(); i++)
	{
		if (vector.at(i).m_t > 0.0f)
		{
			return vector.at(i);
		}
	}
	return {0.0f, *vector.at(0).m_obj};
}

void Intersect(Object* obj, const Ray& R, INTERSECTIONS vector[])
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

void FullIntersection(Object* obj, const Ray& R, std::vector<INTERSECTIONS>& vector)
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
			vector.emplace_back(0.0f, *obj);
			vector.emplace_back(0.0f, *obj);
			return;
		}

		const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
		const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

		vector.emplace_back(std::min(i1,i2), *obj);
		vector.emplace_back(std::max(i1,i2), *obj);
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
			vector.emplace_back(0.0f, *obj);
			vector.emplace_back(0.0f, *obj);
			return;
		}

		const float i1 = (-b - std::sqrtf(discriminant)) / (2.0f * a);
		const float i2 = (-b + std::sqrtf(discriminant)) / (2.0f * a);

		vector.emplace_back(std::min(i1, i2), *obj);
		vector.emplace_back(std::max(i1, i2), *obj);
	}
}

const void SortIntersections(std::vector<INTERSECTIONS>& vector)
{
	std::sort(vector.begin(), vector.end());
	//__debugbreak();
}