#ifndef OBJECTS_H
#define OBJECTS_H
#include "Ray.h"
#include "Tuples.h"
#include "Materials.h"
#include "Lights.h"
#include "Utils.h"
#include <vector>
#include <algorithm>

class Object
{
public:
	Object() = default;
	virtual ~Object() {}
	virtual void SetTransform(const Matrix4x4f& t) = 0;
	virtual const uint64_t GetId() = 0;

protected:
	static uint64_t GenerateId()
	{
		static uint64_t s_Id = 0;
		return s_Id++;
	}
};

class Sphere : public Object
{
public:
	Sphere() : m_Id(GenerateId()), m_transform(IdentityMat4x4f()), m_MaterialType(DefaultMaterial) {}
	Sphere(const Materials::Materials& mat) { m_Id = GenerateId(); m_transform = IdentityMat4x4f(); m_MaterialType = mat; }

	void SetTransform(const Matrix4x4f& t) override { m_transform = t; }
	const void SetMaterial(const Materials::Materials& material) { m_MaterialType = material; }
	inline const Materials::Materials GetMaterial() const { return m_MaterialType; }
	const Matrix4x4f GetTransform() const { return m_transform; }
	const uint64_t GetId() override { return m_Id; }
	const Tuple::Pos NormalAt(Tuple::Pos point) const
	{
		const Matrix4x4f x{ m_transform.Invert() };
		const Tuple::Pos object_point{ x * point };
		const Tuple::Pos object_normal { object_point - Tuple::Point(0, 0, 0)};

		Tuple::Pos world_normal { x.Transposed() * object_normal};
		world_normal.m_type = 0;
		return Normalize(world_normal);
	}

private:
	Matrix4x4f m_transform;
	uint64_t m_Id;
	Materials::Materials m_MaterialType;
};

class World
{
public:
	World() {}
	~World() {}

	void AddObject(Object * obj) { /*m_ObjVector->emplace_back(&obj);*/ }
	void DefaultWorld()
	{
		m_Lumini = { Tuple::Point(-10.0f, 10.0f, -10.0f), WHITE }; // world light

		m_ObjVector.emplace_back(new Sphere());
		m_ObjVector.emplace_back(new Sphere());

		static_cast<Sphere*>(m_ObjVector[0])->SetMaterial(Materials::Materials{Color{0.8f, 1.0f, 0.6f}, 0.0f, 0.7f, 0.2f, 0.0f}); // Sphere 0 color
		static_cast<Sphere*>(m_ObjVector[1])->SetTransform(Scale(0.5f, 0.5f, 0.5f)); // Sphere 1 scale
	}
	inline const void SetLight(const Light & light) { m_Lumini = light;}
	inline const Light GetLight() const { return m_Lumini; }
	inline const std::vector<Object*> getObjVector() const { return m_ObjVector;}

private:
	std::vector<Object*> m_ObjVector;
	Light m_Lumini;
};

#endif