#include "../SourceHeaders/Canvas.h"
#include "../SourceHeaders/Matrixes.h"
#include "../SourceHeaders/Ray.h"
#include "../SourceHeaders/Objects.h"
#include "../SourceHeaders/Utils.h"
#include "../SourceHeaders/Lights.h"
#include "../SourceHeaders/Intersections.h"
#include <algorithm>
#include <memory>
#include <cmath>
#define Pt Tuple::Point
#define Vec Tuple::Vector

void Chapter5(int x = 480, int y = 480)
{
	const auto start = std::chrono::high_resolution_clock::now();

	//INTERSECTIONS* vector = new INTERSECTIONS[static_cast<size_t>(x * y) + 1ull]; // intellisense warnings. . .
	Utils::Vector<INTERSECTIONS> vector((x * y)+1);
	Sphere s;

	const auto ray_origin{ Tuple::Point(0,0,-5) };
	const float wall_z = 10.0f;
	const float wall_size = 7.0f;

	Canvas canv(x, y);
	const float pixel_sizeHeight = wall_size / static_cast<float>(canv.Height());
	const float pixel_sizeWidth = wall_size / static_cast<float>(canv.Width());
	const float halfHeight = pixel_sizeHeight * 0.5f;
	const float halfWidth = pixel_sizeWidth * 0.5f;

	const auto mat = Translate(3.0f, -3.0f, 8.0f);
	s.SetTransform(mat);

	for (int32_t i = 0; i < canv.Height(); i++)
	{
		float worldy = halfHeight - pixel_sizeHeight * static_cast<float>(i);
		for (int32_t j = 0; j < canv.Width(); j++)
		{
			float worldx = -halfWidth + pixel_sizeWidth * static_cast<float>(j);
			Tuple::Pos position = Tuple::Point(worldx, worldy, wall_z);
			Ray r{ ray_origin, Tuple::Normalize(position - ray_origin) };
			Intersect(&s, r, vector);
			const float t = vector[(int)i * canv.Height() + j].m_t;
			if (t > 0)
				canv.WritePixel(i, j, RED);
			else
				canv.WritePixel(i, j, BLACK);
		}
	}
	canv.ExportAsPPM();

	// time measurement .end
	const auto end = std::chrono::high_resolution_clock::now();

	// time calculation
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	std::cout << "Elapsed time: " << duration << " microseconds!" << std::endl;
	system("Files\\RAYTRACER.ppm");
}
void Chapter6(int x = 480, int y = 480)
{
	Canvas canv(x, y);
	//INTERSECTIONS* vector = new INTERSECTIONS[(x * y) + 1]; // +1 because intellisense is annoying
	Utils::Vector<INTERSECTIONS> vector((x * y) + 1);

	// start timer
	const auto start = std::chrono::high_resolution_clock::now();

	const Light light{ Tuple::Point(-10.0f, 10.0f, -10.0f), WHITE };
	Sphere s;
	s.SetMaterial({ RED, 0.1f, 0.9f, 0.9f, 75.0f });

	const auto ray_origin{ Tuple::Point(0,0,-5) };
	const int wall_z = 10;
	const int wall_size = 7;

	const float pixel_sizeHeight = static_cast<float>(wall_size) / static_cast<float>(canv.Height());
	const float pixel_sizeWidth = static_cast<float>(wall_size) / static_cast<float>(canv.Width());
	const float halfHeight = pixel_sizeHeight * 0.5f;
	const float halfWidth = pixel_sizeWidth * 0.5f;

	const auto mat = Translate(3.0f, -3.0f, 8.0f);
	s.SetTransform(mat);

	// Intersect ray with sphere
	for (int32_t i = 0; i < canv.Height(); i++)
	{
		const float worldy = halfHeight - pixel_sizeHeight * static_cast<float>(i);
		for (int32_t j = 0; j < canv.Width(); j++)
		{
			const float worldx = -halfWidth + pixel_sizeWidth * static_cast<float>(j);
			const Tuple::Pos position = Tuple::Point(worldx, worldy, static_cast<float>(wall_z));
			const Ray r{ ray_origin, Tuple::Normalize(position - ray_origin) };
			Intersect(&s, r, vector);
			const float t = vector[(int)i * canv.Height() + j].m_t;

			if (t > 0)
				canv.WritePixel(i, j, Lighting(s.GetMaterial(), light, r.Position(t), -r.GetDirection(), s.NormalAt(r.Position(t))));
			else
				canv.WritePixel(i, j, BLACK);
		}
	}
	canv.ExportAsPPM();

	// Time measurement
	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::cout << "Elapsed time: " << duration << " microseconds!" << std::endl;

	system("Files\\RAYTRACER.ppm");
}

Color TesteColor_at(World * world, const Ray& ray, std::vector<INTERSECTIONS> vector)
{
	IntersectWorld(world, ray, vector);
	SortIntersections(vector);

	// If Hit()
	for (int i = 0; i < vector.size(); i++)
	{
		if (vector.at(i).m_t > 0)
			return Shade_Hit(*world, {ray, vector[(int)i]});
	}
	return BLACK;
	// if didn't Hit
}

void TesteFunc()
{
	std::shared_ptr<World> w = std::make_shared<World>(); // stack overflow if not heap-allocated
	w.get()->DefaultWorld();
	std::vector<INTERSECTIONS> vector;
	Ray r{ Pt(0.0f, 0.0f, -5.0f), Vec(0.0f, 0.0f, 1.0f) };

	Color col = TesteColor_at(w.get(), r, vector);
	std::cout << col;
}

int main()
{	
	//CanvasTest();
	//Chapter5();
	Chapter6();
	//TesteFunc();

	return 0;
}