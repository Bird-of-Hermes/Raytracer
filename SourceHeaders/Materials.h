#ifndef MATERIALS_H
#define MATERIALS_H
#include "Tuples.h"

namespace Materials 
{
	struct Materials
	{
		Materials() : m_Color(WHITE), m_Ambient(0.0f), m_Diffuse(0.0f), m_Specular(0.0f), m_Shininess(0.0f){}
		Materials(const Color& color, const float ambient = 0.0f, const float diffuse = 0.0f, const float specular = 0.0f, const float shininess = 0.0f)
		{
			m_Color = color; m_Ambient = ambient; m_Diffuse = diffuse; m_Specular = specular; m_Shininess = shininess;
		}
		Color m_Color;
		float m_Ambient;
		float m_Diffuse;
		float m_Specular;
		float m_Shininess;
	};
}
#define DefaultMaterial Materials::Materials{{WHITE}, 0.1f, 0.9f, 0.9f, 200.0f}

#endif