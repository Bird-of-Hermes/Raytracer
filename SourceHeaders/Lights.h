#ifndef LIGHTS_H
#define LIGHTS_H
#include "Tuples.h"
#include "Patterns.h"
#include "Materials.h"
#include <cmath>

class Light
{
public:	
	inline Color const GetIntensity() const { return m_Intensity; }
	inline const Tuple::Pos GetPosition() const { return m_Position; }
	inline void SetIntensity(const Color& color) { m_Intensity = color; }
	inline void SetPosition(const Tuple::Pos& point) { m_Position = point; }

	Light() : m_Intensity(WHITE), m_Position(Tuple::Point(0, 0, 0)) {}
	Light(const Tuple::Pos& position, const Color& intensity) : m_Intensity(intensity), m_Position(position){}

private:
	Tuple::Pos m_Position;
	Color m_Intensity;
};

inline const Color Lighting(const Materials::Materials& mat, Light light, Tuple::Pos point, Tuple::Pos eyev, Tuple::Pos normalv, bool is_shadow)
{
	Color diffuse, specular;
	Color effective_color;

	if (mat.m_haspattern == true)
		effective_color = mat.m_pattern.StripeAt(point) * light.GetIntensity();
	
	effective_color = mat.m_Color * light.GetIntensity();

	const Color ambient = effective_color * mat.m_Ambient;
	if (is_shadow)
	{
		return ambient;
	}
	const Tuple::Pos lightv = Normalize(light.GetPosition() - point);
	const float light_dot_normal = Tuple::DotProduct(lightv, normalv);

	if (light_dot_normal >= 0)
	{
		diffuse = effective_color * mat.m_Diffuse * light_dot_normal;
		const Tuple::Pos reflectv = Tuple::Reflect(-lightv, normalv);
		const float reflect_dot_eye = Tuple::DotProduct(reflectv, eyev);
		
		if (reflect_dot_eye > 0)
		{
			const float factor = std::powf(reflect_dot_eye, mat.m_Shininess);
			specular = light.GetIntensity() * mat.m_Specular * factor;
		}
	}
	return ambient + diffuse + specular;
}

#endif