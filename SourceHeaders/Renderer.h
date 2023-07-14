#ifndef RENDERER_H
#define RENDERER_H

#include <execution>
#include "Timer.h"
#include "Ray.h"
#include "Canvas.h"
#include "Intersections.h"
#include "Filters.h"

inline const void Render(Camera& cam, World& world)
{
	Canvas img(cam.GetWidth(), cam.GetHeight()); // original img
	Canvas img2(cam.GetWidth(), cam.GetHeight()); // median filter
	Canvas img3(cam.GetWidth(), cam.GetHeight()); // sobel filter

	// iterators
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
			std::for_each(std::execution::par, WidthIter.begin(), WidthIter.end(), [&, y](uint32_t x)
				{
					const Ray r{ RayForPixel(cam, x, y) };
					//const Color cor{ HeapColorAt(&world, r) };
					const Color cor{ ColorAt(&world, r) };
					img.WritePixel(y, x, cor);
					//Mem.Reload(100'000ll);
				});
		});
	hot.now();

	img.ExportAsPPM("Files/RAYTRACER.ppm");
	MedianFilter(&img, 2, &img2); // median filter -> sobel operator -> AAFilter -> originalimg
	SobelOperator(&img2, &img3);
	AAFilter(&img2, &img3);
	BlendMultiply(&img);
}
#endif