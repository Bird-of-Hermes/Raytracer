#ifndef OBJECTS_H
#define OBJECTS_H
#include "Tuples.h"
#include "Materials.h"
#include "Lights.h"
#include "Utils.h"
#include <vector>
#include <algorithm>

enum class TYPE : uint32_t{SPHERE = 0, PLANE};

class Object
{
public:
	Object() = default;
	virtual ~Object() {}
	virtual const Matrix4x4f GetInvTransform() const = 0;
	virtual const Matrix4x4f GetTransform() const = 0;
	virtual Materials::Materials* GetMaterial() = 0;
	virtual void SetTransform(const Matrix4x4f& t) = 0;
	virtual const void SetMaterial(const Materials::Materials& material) = 0;
	virtual const uint32_t GetId() const = 0;
	virtual const TYPE GetType() const = 0;

protected:
	static uint32_t GenerateId()
	{
		static uint32_t s_Id = 0;
		return s_Id++;
	}
};

class Sphere : public Object
{
public:
	Sphere() : m_Id(GenerateId()), m_transform(IdentityMat4x4f()), m_InvTransform(IdentityMat4x4f()), m_MaterialType(DefaultMaterial) {}
	Sphere(const Materials::Materials& mat) { m_Id = GenerateId(); m_transform = IdentityMat4x4f(); m_MaterialType = mat; }
	~Sphere() {}

	void SetTransform(const Matrix4x4f& t) override { m_transform = t; m_InvTransform = m_transform.Invert(); }
	const void SetMaterial(const Materials::Materials& material) { m_MaterialType = material; }
	Materials::Materials* GetMaterial() { return &m_MaterialType; }
	const Matrix4x4f GetInvTransform() const { return m_InvTransform; }
	const Matrix4x4f GetTransform() const { return m_transform; }
	const uint32_t GetId() const override { return m_Id; }
	const TYPE GetType() const override { return TYPE::SPHERE; }

private:
	Matrix4x4f m_transform;
	Matrix4x4f m_InvTransform;
	uint32_t m_Id;
	Materials::Materials m_MaterialType;
};

class Plane : public Object
{
public:
	Plane() : m_Id(GenerateId()), m_transform(IdentityMat4x4f()), m_InvTransform(IdentityMat4x4f()), m_MaterialType(DefaultMaterial) {}
	Plane(const Materials::Materials& mat) { m_Id = GenerateId(); m_transform = IdentityMat4x4f(); m_MaterialType = mat; }
	~Plane() {}

	void SetTransform(const Matrix4x4f& t) override { m_transform = t; m_InvTransform = m_transform.Invert();}
	const void SetMaterial(const Materials::Materials& material) { m_MaterialType = material; }
	Materials::Materials* GetMaterial() { return &m_MaterialType; }
	const Matrix4x4f GetInvTransform() const { return m_InvTransform; }
	const Matrix4x4f GetTransform() const { return m_transform; }
	const uint32_t GetId() const override { return m_Id; }
	const TYPE GetType() const override { return TYPE::PLANE; }

private:
	Matrix4x4f m_transform;
	Matrix4x4f m_InvTransform;
	uint32_t m_Id;
	Materials::Materials m_MaterialType;
};

class World
{
public:
	World() {}
	~World() {}

	const void AddObject(Object * obj) { m_ObjVector.emplace_newptr(obj); }
	const void DefaultWorld()
	{
		m_Lumini = { Tuple::Point(-10.0f, 10.0f, -10.0f), WHITE }; // world light

		m_ObjVector.emplace_newptr(new Sphere());
		m_ObjVector.emplace_newptr(new Sphere());

		static_cast<Sphere*>(m_ObjVector[0])->SetMaterial(Materials::Materials{Color{0.8f, 1.0f, 0.6f}, 0.0f, 0.7f, 0.2f, 0.0f}); // Sphere 0 color
		static_cast<Sphere*>(m_ObjVector[1])->SetTransform(Scale(0.5f, 0.5f, 0.5f)); // Sphere 1 scale
	}
	constexpr void SetLight(const Light & light) { m_Lumini = light;}
	const Light GetLight() const { return m_Lumini; }
	const Utils::Vector<Object*> getObjVector() const { return m_ObjVector;}

private:
	Utils::Vector<Object*> m_ObjVector;
	Light m_Lumini;
};

class Camera
{
public:
	Camera() 
	{
		m_hsize = 600.0f; m_vsize = 600.0f; m_fov = toRadians(180.0f); m_transform = IdentityMat4x4f();

		auto halfview = std::tanf(m_fov / 2.0f);
		float halfwidth, halfheight;
		auto aspect = m_hsize / m_vsize;
		if (aspect >= 1.0f)
		{
			halfwidth = halfview;
			halfheight = halfview / aspect;
		}
		else
		{
			halfwidth = halfview * aspect;
			halfheight = halfview;
		}
		m_pixelsize = (halfwidth * 2.0f) / m_hsize;

	}
	Camera(float width, float height, float fieldofview) : m_hsize(width), m_vsize(height), m_fov(fieldofview) 
	{
		m_transform = IdentityMat4x4f();

		m_Arr_halfVWH[0] = std::tanf(m_fov / 2.0f);
		
		const float aspect = m_hsize / m_vsize;
		if (aspect >= 1.0f)
		{
			m_Arr_halfVWH[1] = m_Arr_halfVWH[0];
			m_Arr_halfVWH[2] = m_Arr_halfVWH[0] / aspect;
		}
		else
		{
			m_Arr_halfVWH[1] = m_Arr_halfVWH[0] * aspect;
			m_Arr_halfVWH[2] = m_Arr_halfVWH[0];
		}
		m_pixelsize = (m_Arr_halfVWH[1] * 2.0f) / m_hsize;
	}

	const Matrix4x4f GetTransform() const { return m_transform; }
	constexpr uint32_t GetWidth() const { return static_cast<uint32_t>(m_hsize); }
	constexpr uint32_t GetHeight() const { return static_cast<uint32_t>(m_vsize); }
	constexpr float GetFieldOfView() const { return m_fov; }
	constexpr float GetPixelSize() const { return m_pixelsize; }
	const float* GetHalfsArray() const { return m_Arr_halfVWH; }
	const void SetTransform(const Matrix4x4f& transform) { m_transform = transform; }
	constexpr void SetWidth(float width) { m_hsize = width; }
	constexpr void SetHeight(float height) { m_vsize = height; }
	constexpr void SetFieldOfView(float fov) { m_fov = fov; }

private:
	Matrix4x4f m_transform;
	float m_hsize;
	float m_vsize;
	float m_fov;
	float m_pixelsize;
	float m_Arr_halfVWH[3];
};

inline const Tuple::Pos NormalAt(Object* obj, Tuple::Pos point)
{
	if (obj->GetType() == TYPE::PLANE)
	{
		return Tuple::Vector(0, 1, 0);
	}
// if it's a translation transform, just return the xyz vector
//	if ((x[3] != 0 || x[7] != 0 || x[11] != 0) && obj->GetType() == TYPE::PLANE)
//		return Tuple::Pos(x[3], x[7], x[11]);
	else
	{		
		const Tuple::Pos object_point { obj->GetInvTransform() * point };
		const Tuple::Pos object_normal { object_point - Tuple::Point(0, 0, 0)};

		Tuple::Pos world_normal { obj->GetInvTransform().Transposed() * object_normal};
		world_normal.m_type = 0;
		return Normalize(world_normal);
	}
};

inline const Color StripeAtObject(Object* obj, PATTERNS pattern, Tuple::Pos worldpoint)
{
	const Tuple::Pos objpoint {obj->GetInvTransform() * worldpoint};
	const Tuple::Pos patpoint {pattern.GetInvTransform() * objpoint};
	return pattern.StripeAt(patpoint);
}

#endif