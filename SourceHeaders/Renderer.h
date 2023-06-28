#ifndef RENDERER_H
#define RENDERER_H

#include "Ray.h"
#include "Canvas.h"
#include "Intersections.h"

inline const void Render(Camera& cam, World& world)
{
	Canvas img(static_cast<uint32_t>(cam.GetWidth()), static_cast<uint32_t>(cam.GetHeight()));

	for (uint32_t y = 0; y < cam.GetHeight() - 1; y++)
	{
		for (uint32_t x = 0; x < cam.GetWidth() - 1; x++)
		{
			std::vector<INTERSECTIONS> intersections;
			const Ray r{ RayForPixel(cam, x, y) };
			const Color cor{ ColorAt(&world, r, intersections) };
			img.WritePixel(y, x, cor);
		}
	}
	img.ExportAsPPM();
}

#endif