#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Tuples.h"

struct Projectile
{
	Tuple::Pos m_coord;
	Tuple::Pos velocity;

	Projectile() : m_coord(0), velocity(0) {}
	Projectile(Tuple::Pos coord, Tuple::Pos vel) : velocity(vel) { m_coord = Tuple::Point(coord);}
	Projectile(float x, float y, float z, Tuple::Pos vel) : velocity(vel) { m_coord = Tuple::Point(x, y, z); }
	Projectile(float x, float y, float z, float vx, float vy, float vz) : velocity(vx,vy,vz) { m_coord = Tuple::Point(x, y, z); }

	Projectile operator+(const Projectile& b) { return { this->m_coord + b.m_coord, this->velocity + b.velocity }; }
};

struct Environment
{
	Tuple::Pos m_gravity;
	Tuple::Pos m_wind;

	Environment(Tuple::Pos gravity, Tuple::Pos wind) : m_gravity(gravity), m_wind(wind) {} // gravity and wind
	Environment(float gx, float gy, float gz, float wx, float wy, float wz) : m_gravity(gx, gy, gz), m_wind(wx, wy, wz) {}
};

inline Projectile tick(Environment env, Projectile proj) 
{ 
	const Tuple::Pos position = proj.m_coord + proj.velocity;
	const Tuple::Pos velocity = proj.velocity + env.m_gravity + env.m_wind;
	
	return { position, velocity };
}
#endif