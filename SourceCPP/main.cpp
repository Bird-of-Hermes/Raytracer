#include "../SourceHeaders/Canvas.h"
#include "../SourceHeaders/Matrixes.h"
#include "../SourceHeaders/Ray.h"
#include "../SourceHeaders/Objects.h"
#include "../SourceHeaders/Utils.h"
#include "../SourceHeaders/Lights.h"
#include "../SourceHeaders/Intersections.h"
#include "../SourceHeaders/Timer.h"
#include "../SourceHeaders/Renderer.h"
#include <algorithm>
#include <memory>
#include <cmath>
#define Pt Tuple::Point
#define Vec Tuple::Vector

void Chapter5(int x = 480, int y = 480)
{
	const auto start = std::chrono::high_resolution_clock::now();
	
	//INTERSECTIONS* vector = new INTERSECTIONS[static_cast<size_t>(x * y) + 1ull]; // intellisense warnings. . .
	Utils::Vector<INTERSECTIONS> vector(static_cast<size_t>(x * y)+1ull);
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
			const float t = vector[static_cast<size_t>(i * canv.Height() + j)].m_t;
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
			const float t = vector[static_cast<size_t>(i * canv.Height() + j)].m_t;
			if (t > 0)
				canv.WritePixel(i, j, Lighting(s.GetMaterial(), light, r.Position(t), -r.GetDirection(), s.NormalAt(r.Position(t)), false));
			else
				canv.WritePixel(i, j, BLACK);
		}
	}
	canv.ExportAsPPM();
}
void Chapter7(float x = 480, float y = 480)
{
	World world;

	world.AddObject(new Sphere()); // floor [0]
	const Materials::Materials mat {{0.5f, 0.5f, 0.5f}, 0.1f, 0.9f, 0.0f, 100.0f}; // floor/wall material
	world.getObjVector()[0]->SetTransform(Scale(10.0f, 0.01f, 10.0f));
	world.getObjVector()[0]->SetMaterial(mat);
	
	world.AddObject(new Sphere()); // left_wall [1]
	world.getObjVector()[1]->SetTransform(Translate(0, 0, 5) * RotateYaxis(-45.0f) * RotateXaxis(90.0f) * Scale(10.0f, 0.01f, 10.0f));
	world.getObjVector()[1]->SetMaterial(mat);

	world.AddObject(new Sphere()); // right_wall [2]
	world.getObjVector()[2]->SetTransform(Translate(0, 0, 5) * RotateYaxis(45.0f) * RotateXaxis(90.0f) * Scale(10.0f, 0.01f, 10.0f));
	world.getObjVector()[2]->SetMaterial(mat);

	world.AddObject(new Sphere()); // middle [3]
	world.getObjVector()[3]->SetTransform(Translate(-0.5f, 1.0f, 0.5f));
	const Materials::Materials middlemat {{0.1f, 1.0f, 0.1f}, 0.1f, 0.7f, 0.3f, 100.0f};
	world.getObjVector()[3]->SetMaterial(middlemat);

	world.AddObject(new Sphere()); // right [4]
	world.getObjVector()[4]->SetTransform(Translate(1.5f, 0.5f, -0.5f) * Scale(0.5f, 0.5f, 0.5f));
	const Materials::Materials rightmat {{0.5f, 1.0f, 0.1f}, 0.1f, 0.7f, 0.3f, 100.0f};
	world.getObjVector()[4]->SetMaterial(rightmat);

	world.AddObject(new Sphere()); // left [5]
	world.getObjVector()[5]->SetTransform(Translate(-1.5f, 0.33f, -0.75f) * Scale(0.33f, 0.33f, 0.33f));
	const Materials::Materials leftmat {{1.0f, 0.8f, 0.1f}, 0.1f, 0.7f, 0.3f, 100.0f};
	world.getObjVector()[5]->SetMaterial(leftmat);
	
	// world light
	world.SetLight({ Pt(-10.0f,10.0f,-10.0f), WHITE });

	Camera camera{ x, y, 95.8f };
	camera.SetTransform(ViewTransform(Pt(0, 1.5f, -5.0f), Pt(0, 1, 0), Vec(0, 1, 0)));

	Render(camera, world);
}
void Testes()
{
	const auto eyev{ Vec(0,0,1) };
	const auto normalv{ Vec(0,0,1) };
	const Light lightv{ Pt(0,0,-10),{1,1,1} };
	const bool in_shadow = true;
	const auto point{ Pt(0,0,0) };
	const auto m{ DefaultMaterial };
	const auto result = Lighting(m, lightv, point, eyev, normalv, in_shadow);
	std::cout << result << std::endl;
}

int main()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ TIMER ~~~~~~~~~~~~~~~~~~~~~~~~~//
	//const Timer timer;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
	
	//CanvasTest();
	//Chapter5();
	//Chapter6();
	Chapter7(1920, 1080);

	//system("Files\\RAYTRACER.ppm");
	//Testes();

	return 0;
}