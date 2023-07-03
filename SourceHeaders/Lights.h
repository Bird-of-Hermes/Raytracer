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
#endif