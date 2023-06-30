#ifndef RENDERER_H
#define RENDERER_H

#include <execution>
#include "Timer.h"
#include "Ray.h"
#include "Canvas.h"
#include "Intersections.h"

inline const void Render(Camera& cam, World& world)
{
	Canvas img(cam.GetWidth(), cam.GetHeight());

	std::vector<uint32_t>HeightIter(cam.GetHeight());
	std::vector<uint32_t>WidthIter(cam.GetWidth());
	for (uint32_t i = 0; i < cam.GetHeight(); ++i) 
	{
		HeightIter[i] = i;
	}
	for (uint32_t i = 0; i < cam.GetWidth(); i++)
	{
		WidthIter[i] = i;
	}
	
	Timer hot;
	std::for_each(std::execution::par, HeightIter.begin(), HeightIter.end(), [&](uint32_t y)
		{
			std::for_each(std::execution::par, WidthIter.begin(), WidthIter.end(), [&,y](uint32_t x)
				{
					Utils::Static_Array<INTERSECTIONS, 12> inters;
					const Ray r{ RayForPixel(cam, x, y) };
					//IsShadow(&world, r.GetOrigin());
					const Color cor{ ColorAt(&world, r, inters) };
					img.WritePixel(y, x, cor);
				});
		});
	hot.now();

	img.ExportAsPPM();
}
#endif