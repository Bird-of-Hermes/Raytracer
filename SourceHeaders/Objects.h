#ifndef OBJECTS_H
#define OBJECTS_H
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
	virtual const void SetMaterial(const Materials::Materials& material) = 0;
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

	constexpr void AddObject(Object * obj) { m_ObjVector.emplace_back(obj); }
	const void DefaultWorld()
	{
		m_Lumini = { Tuple::Point(-10.0f, 10.0f, -10.0f), WHITE }; // world light

		m_ObjVector.emplace_back(new Sphere());
		m_ObjVector.emplace_back(new Sphere());

		static_cast<Sphere*>(m_ObjVector[0])->SetMaterial(Materials::Materials{Color{0.8f, 1.0f, 0.6f}, 0.0f, 0.7f, 0.2f, 0.0f}); // Sphere 0 color
		static_cast<Sphere*>(m_ObjVector[1])->SetTransform(Scale(0.5f, 0.5f, 0.5f)); // Sphere 1 scale
	}
	constexpr void SetLight(const Light & light) { m_Lumini = light;}
	const Light GetLight() const { return m_Lumini; }
	constexpr std::vector<Object*> getObjVector() const { return m_ObjVector;}

private:
	std::vector<Object*> m_ObjVector;
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

	inline const Matrix4x4f GetTransform() const { return m_transform; }
	inline const float GetWidth() const { return m_hsize; }
	inline const float GetHeight() const { return m_vsize; }
	inline const float GetFieldOfView() const { return m_fov; }
	inline const float GetPixelSize() const { return m_pixelsize; }
	inline const float* GetHalfsArray() const { return m_Arr_halfVWH; }
	inline const void SetTransform(const Matrix4x4f& transform) { m_transform = transform; }
	inline const void SetWidth(float width) { m_hsize = width; }
	inline const void SetHeight(float height) { m_vsize = height; }
	inline const void SetFieldOfView(float fov) { m_fov = fov; }

private:
	Matrix4x4f m_transform;
	float m_hsize;
	float m_vsize;
	float m_fov;
	float m_pixelsize;
	float m_Arr_halfVWH[3];
};

#endif