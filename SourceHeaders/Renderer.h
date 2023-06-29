#ifndef RENDERER_H
#define RENDERER_H

#include "Timer.h"
#include "Ray.h"
#include "Canvas.h"
#include "Intersections.h"

inline const void Render(Camera& cam, World& world)
{
	Canvas img(static_cast<uint32_t>(cam.GetWidth()), static_cast<uint32_t>(cam.GetHeight()));
	Utils::Vector<INTERSECTIONS> intersections(world.getObjVector().size()*2*(cam.GetWidth() * cam.GetHeight()));

#if false
	for (uint32_t y = 0; y < cam.GetHeight() - 1; y++)
	{
		for (uint32_t x = 0; x < cam.GetWidth() - 1; x++)
		{
			const Ray r{ RayForPixel(cam, x, y) };
			const Color cor{ ColorAt(&world, r, intersections) };
			img.WritePixel(y, x, cor);
		}
	}
	img.ExportAsPPM();
#endif

#if true
#pragma omp parallel for num_threads(12)
	for (int i = 0; i < static_cast<int>((cam.GetHeight() - 1) * (cam.GetWidth() - 1)); i++)
	{
		const uint32_t y = i / static_cast<uint32_t>((cam.GetWidth() - 1));
		const uint32_t x = i % static_cast<uint32_t>((cam.GetWidth() - 1));

		const Ray r{ RayForPixel(cam, x, y) };
		const Color cor{ ColorAt(&world, r, intersections) };

#pragma omp critical
		{
			img.WritePixel(y, x, cor);
		}
	}
#endif
}
#endif