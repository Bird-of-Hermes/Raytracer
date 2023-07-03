#include "../SourceHeaders/Canvas.h"
#include "../SourceHeaders/Matrixes.h"
#include "../SourceHeaders/Ray.h"
#include "../SourceHeaders/Objects.h"
#include "../SourceHeaders/Utils.h"
#include "../SourceHeaders/Lights.h"
#include "../SourceHeaders/Intersections.h"
#include "../SourceHeaders/Timer.h"
#include "../SourceHeaders/Renderer.h"
#include "../SourceHeaders/Patterns.h"
#include <algorithm>
#include <memory>
#include <cmath>
#define Pt Tuple::Point
#define Vec Tuple::Vector

#define SD 720.0f, 480.0f
#define HD 1280.0f, 720.0f
#define FULLHD 1920.0f, 1080.0f
#define UHD 3840.0f, 2160.0f
#define MAXHD 7680.0f, 4320.0f
#define PI 180.0f

void Chapter5(int x = 480, int y = 480)
{
	const auto start = std::chrono::high_resolution_clock::now();
	
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
void Chapter7(float x = 720, float y = 480)
{
	World world;

	const Materials::Materials mat {{(PINK+TURQUOISE)/4.0f}, 0.1f, 0.9f, 0.05f, 100.0f}; // floor/wall material
	world.AddObject(new Sphere()); // floor [0]
	world.getObjVector()[0]->SetTransform(Scale(10.0f, 0.01f, 10.0f));
	world.getObjVector()[0]->SetMaterial(mat);
	
	world.AddObject(new Sphere()); // left_wall [1]
	world.getObjVector()[1]->SetTransform(Translate(0, 0, 5) * RotateYaxis(-45.0f) * RotateXaxis(90.0f) * Scale(10.0f, 0.01f, 10.0f));
	world.getObjVector()[1]->SetMaterial(mat);

	world.AddObject(new Sphere()); // right_wall [2]
	world.getObjVector()[2]->SetTransform(Translate(0, 0, 5) * RotateYaxis(45.0f) * RotateXaxis(90.0f) * Scale(10.0f, 0.01f, 10.0f));
	world.getObjVector()[2]->SetMaterial(mat);

	const Materials::Materials middlemat {{TOMATORED}, 0.1f, 0.7f, 0.3f, 100.0f};
	world.AddObject(new Sphere()); // middle [3]
	world.getObjVector()[3]->SetTransform(Translate(-0.5f, 1.0f, 0.5f));
	world.getObjVector()[3]->SetMaterial(middlemat);

	const Materials::Materials rightmat {{0.125f, 0.125f, 0.125f}, 0.1f, 0.7f, 0.3f, 100.0f}; // petroleum
	world.AddObject(new Sphere()); // right [4]
	world.getObjVector()[4]->SetTransform(Translate(1.5f, 0.5f, -0.5f) * Scale(0.5f, 0.5f, 0.5f));
	world.getObjVector()[4]->SetMaterial(PETROLEUM);

	const Materials::Materials leftmat {{CYAN}, 0.1f, 0.7f, 0.3f, 100.0f};
	world.AddObject(new Sphere()); // left [5]
	world.getObjVector()[5]->SetTransform(Translate(-1.5f, 0.33f, -0.75f) * Scale(0.33f, 0.33f, 0.33f));
	world.getObjVector()[5]->SetMaterial(leftmat);
	
	// world light
	world.SetLight({ Pt(-10.0f,10.0f,-10.0f), WHITE });

	Camera camera{ x, y, 95.8f };
	camera.SetTransform(ViewTransform(Pt(0, 1.5f, -5.0f), Pt(0, 1, 0), Vec(0, 1, 0)));

	Render(camera, world);
}
void Chapter9(float x = 720, float y = 480)
{
	World world;

	const Materials::Materials middlemat {{TOMATORED}, 0.1f, 0.7f, 0.3f, 100.0f};
	world.AddObject(new Sphere()); // middle sphere [0]
	world.getObjVector()[0]->SetTransform(Translate(-0.5f, 1.0f, 0.5f));
	world.getObjVector()[0]->SetMaterial(middlemat);

	const Materials::Materials rightmat {{0.125f, 0.125f, 0.125f}, 0.1f, 0.7f, 0.3f, 100.0f}; // petroleum
	world.AddObject(new Sphere()); // right sphere [1]
	world.getObjVector()[1]->SetTransform(Translate(1.5f, 0.5f, -0.5f) * Scale(0.5f, 0.5f, 0.5f));
	world.getObjVector()[1]->SetMaterial(PETROLEUM);

	const Materials::Materials leftmat {{ORANGE}, 0.1f, 0.7f, 0.3f, 100.0f};
	world.AddObject(new Sphere()); // left sphere [2]
	world.getObjVector()[2]->SetTransform(Translate(-1.5f, 0.33f, -0.75f) * Scale(0.33f, 0.33f, 0.33f));
	world.getObjVector()[2]->SetMaterial(leftmat);

	//(TURQUOISE) / 2.0f + WHITE
	const Materials::Materials mat {{(TURQUOISE) / 2.0f + WHITE}, 0.5f, 0.5f, 0.5f, 100.0f}; // floor/wall material
	world.AddObject(new Plane()); // floor [3]
	world.getObjVector()[3]->SetMaterial(mat);

	// world light
	world.SetLight({ Pt(-10.0f,10.0f,-10.0f), WHITE });

	Camera camera{ x, y, 96.3f };
	camera.SetTransform(ViewTransform(Pt(0, 3.0f, -4.0f), Pt(0, 1, 0), Vec(0, 1, 0)));

	Render(camera, world);
}
void Chapter10(float x = 720, float y = 480)
{
	World world;

	const Materials::Materials middlemat {{ROYALBLUE, VIOLET}, 0.1f, 0.7f, 0.3f, 100.0f};
	world.AddObject(new Sphere()); // middle sphere [0]
	world.getObjVector()[0]->SetTransform(Translate(-0.45f, 1.0f, 0.5f));
	world.getObjVector()[0]->SetMaterial(middlemat);
	world.getObjVector()[0]->GetMaterial()->EnablePattern();
	const auto ptransform1 = Translate(1.5f, 1.5f, 1.5f) * Scale(0.25f, 0.25f, 0.25f);
	world.getObjVector()[0]->GetMaterial()->m_pattern.SetTransform(ptransform1);

	const Materials::Materials rightmat {{0.125f, 0.125f, 0.125f}, 0.1f, 0.7f, 0.3f, 100.0f}; // petroleum
	world.AddObject(new Sphere()); // right sphere [1]
	world.getObjVector()[1]->SetTransform(Translate(1.5f, 0.5f, -0.5f) * Scale(0.5f, 0.5f, 0.5f));
	world.getObjVector()[1]->SetMaterial(PETROLEUM);

	const Materials::Materials leftmat {{ORANGE}, 0.1f, 0.7f, 0.3f, 100.0f};
	world.AddObject(new Sphere()); // left sphere [2]
	world.getObjVector()[2]->SetTransform(Translate(-1.5f, 0.33f, -0.75f) * Scale(0.33f, 0.33f, 0.33f));
	world.getObjVector()[2]->SetMaterial(leftmat);

	//(TURQUOISE) / 2.0f + WHITE
	const Materials::Materials mat {{SILVER, WHITE}, 0.5f, 0.5f, 0.5f, 100.0f}; // floor/wall material
	world.AddObject(new Plane()); // floor [3]
	world.getObjVector()[3]->SetMaterial(mat);
	const auto ptransform = RotateYaxis(90.0f) * Scale(0.1f, 0.1f, 0.1f);
	world.getObjVector()[3]->GetMaterial()->m_pattern.SetTransform(ptransform);

	// world light
	world.SetLight({ Pt(-10.0f,10.0f,-10.0f), WHITE });

	Camera camera{ x, y, 96.3f };
	camera.SetTransform(ViewTransform(Pt(0, 3.0f, -4.0f), Pt(0, 1, 0), Vec(0, 1, 0)));

	Render(camera, world);
}
void Teste()
{
	Sphere s;
	s.SetTransform(Scale(2, 2, 2));
	s.GetMaterial()->EnablePattern();
	PATTERNS f;
	f.SetTransform(Translate(0.5f,0,0));
	std::cout << StripeAtObject(&s, f, Pt(2.5f, 0, 0));
}

int main()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~ TIMER ~~~~~~~~~~~~~~~~~~~~~~~~~//
	//const Timer timer;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
	// ~~~~~~~~~~ SD(720p, 480) ~~ HD(1280p, 720) ~~ FULLHD(1920p, 1080) ~~ UHD(3840p, 2160) ~~ MAXHD(7680p, 4320) ~~~~~~~~~~~~~~ //
	
	//CanvasTest();
	//Chapter5();
	//Chapter6();
	//Chapter7(MAXHD); // ~4.5s @ MAXHD without inv.transform in memory + cached // 1.9s cached @ same
	//Chapter9(FULLHD);
	Chapter10(MAXHD); // 1.274s
	//Teste();
	
	//system("Files\\RAYTRACER.ppm");

	return 0;
}