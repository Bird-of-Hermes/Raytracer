#ifndef MATERIALS_H
#define MATERIALS_H
#include "Tuples.h"
#include "Patterns.h"

namespace Materials 
{
	struct Materials
	{
		Materials() : m_Color(WHITE), m_Ambient(0.0f), m_Diffuse(0.0f), m_Specular(0.0f), m_Shininess(0.0f), m_haspattern(false){}
		Materials(const Color& color, const float ambient = 0.1f, const float diffuse = 0.9f, const float specular = 0.9f, const float shininess = 200.0f, const float reflectiveness = 0.0f, const float transparency = 0.0f, const float refractiveindex = 1.0f)
		{
			m_Color = color; 
			m_Ambient = ambient; m_Diffuse = diffuse; m_Specular = specular; m_Shininess = shininess; 
			m_Reflectiveness = reflectiveness;
			m_Transparency = transparency; m_RefractiveIndex = refractiveindex;
			m_haspattern = false;
		}
		Materials(const PATTERNS& pattern, const float ambient = 0.1f, const float diffuse = 0.9f, const float specular = 0.9f, const float shininess = 200.0f, const float reflectiveness = 0.0f, const float transparency = 0.0f, const float refractiveindex = 1.0f)
		{
			m_pattern = pattern; m_haspattern = true;
			m_Ambient = ambient; m_Diffuse = diffuse; m_Specular = specular; m_Shininess = shininess;
			m_Reflectiveness = reflectiveness;
			m_Transparency = transparency; m_RefractiveIndex = refractiveindex;
		}

		void EnablePattern() { m_haspattern = true; }
		void DisablePattern() { m_haspattern = false; }
		void SetPattern(const PATTERNS& pat) { m_pattern = pat; }
		void SetAmbient(const float ambient) { m_Ambient = ambient; }
		void SetDiffuse(const float diffuse) { m_Diffuse = diffuse; }
		void SetSpecular(const float specular) { m_Specular = specular; }
		void SetShininess(const float shininess) { m_Diffuse = shininess; }
		void SetReflection(const float reflection) { m_Reflectiveness = reflection; }

		friend std::ostream& operator<<(std::ostream& os, const Materials& mat)
		{
			os << "MATERIAL(A: [" << mat.m_Ambient << "], D: [" << mat.m_Diffuse << "], S: [" << mat.m_Specular << "], Sh: [" << mat.m_Shininess << "], Color: " << mat.m_Color << ")";

			return os;
		}

		bool m_haspattern;
		PATTERNS m_pattern;
		Color m_Color;
		float m_Ambient;
		float m_Diffuse;
		float m_Specular;
		float m_Shininess;
		float m_Reflectiveness = 0.0f;
		float m_Transparency = 0.0f;
		float m_RefractiveIndex = 1.0f;
	};
}
#define DefaultMaterial Materials::Materials{{WHITE}, 0.1f, 0.9f, 0.9f, 200.0f}
#define PETROLEUM Materials::Materials{{0.112f, 0.113f, 0.112f}, 0.1f, 0.7f, 0.3f, 100.0f}
#define GLASS Materials::Materials{{BLACK}, 0.0f, 0.0f, 0.0f, 0.0f, 0.95f, 1.0f, 1.55f}
#endif