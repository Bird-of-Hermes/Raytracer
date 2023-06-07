#ifndef CANVAS_H
#define CANVAS_H
#include "Tuples.h"
#include "Projectile.h"
#include <chrono>
#include <omp.h>
#include <fstream>

class Canvas
{
private:
	int32_t width;
	int32_t height;
	Color** cor;
public:
	const int32_t Width() const { return width; }
	const int32_t Height() const { return height; }
	const Color PixelAt(uint32_t x, uint32_t y) const { return cor[x][y]; }
	inline const void WritePixel(int32_t x, int32_t y, Color color) 
	{	
		std::cout << x << ", " << y << std::endl;
		if (y >= Height() || y < 0 || x >= Width() || x < 0)
			return;

		cor[y][x].r = color.r;
		cor[y][x].g = color.g;
		cor[y][x].b = color.b;
	}

	void ExportAsPPM()
	{
		std::ofstream fon("RAYTRACER.ppm", std::ios::binary);

		if (!fon.is_open())
		{
			std::cerr << "Couldn't write file\n";
			exit(EXIT_FAILURE);
		}

		fon << "P6\n";
		fon << width << ' ' << height << '\n';
		fon << "255\n";

		for (int32_t i = 0; i < height; i++)
		{
			for (int32_t j = 0; j < width; j++)
			{
				fon << static_cast<uint8_t>(std::round(cor[i][j].r * 255.0f));
				fon << static_cast<uint8_t>(std::round(cor[i][j].g * 255.0f));
				fon << static_cast<uint8_t>(std::round(cor[i][j].b * 255.0f));
			}
		}
		fon.close();

		std::cout << "PPM file successfully exported" << std::endl;
	}
	
	Canvas(uint32_t wid, uint32_t hei) : width(wid), height(hei)
	{
		cor = new Color * [height];

		for (int32_t i = 0; i < height; i++)
			cor[i] = new Color[width];
		
		Color ff {};

		int max_threads1 = omp_get_max_threads();

		auto start = std::chrono::high_resolution_clock::now();
		
		#pragma omp parallel for num_threads(max_threads1)
		for (int32_t i = 0; i < height; i++)
		{
			for (int32_t j = 0; j + 1 < width; j += 12)
			{
				ff = cor[i][j];
				ff = cor[i][j + 1];
				ff = cor[i][j + 2];
				ff = cor[i][j + 3];
				ff = cor[i][j + 4];
				ff = cor[i][j + 5];
				ff = cor[i][j + 6];
				ff = cor[i][j + 7];
				ff = cor[i][j + 8];
				ff = cor[i][j + 9];
				ff = cor[i][j + 10];
				ff = cor[i][j + 11];
			}
		}

		// Fim da medição do tempo
		auto end = std::chrono::high_resolution_clock::now();

		// Cálculo do tempo decorrido em milissegundos
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		std::cout << "Tempo decorrido: " << duration << " microssegundos" << std::endl;
	}
	~Canvas()
	{
		for (int32_t i = 0; i < height; i++)
		{
			delete[] cor[i];
		}
		delete[] cor;
	}
};

void CanvasTest()
{
	Canvas canv(800, 600);

	Projectile proj(Tuple::Point(0.0f, 1.0f, 0.0f), Tuple::Normalize(Tuple::Pos(1.0f, 1.8f, 0.0f)) * 11.25f);
	Environment env(Tuple::Pos(0.0f, -0.1f, 0.0f), Tuple::Pos(-0.01f, 0.0f, 0.0f));

	for (size_t i = 0; i < canv.Height(); i++)
	{
		for (size_t j = 0; j < canv.Width(); j++)
		{
			proj = tick(env, proj);
			if (proj.m_coord.m_y >= canv.Height() || proj.m_coord.m_y < 0 || proj.m_coord.m_x >= canv.Width() || proj.m_coord.m_x < 0)
				continue;
			else
				canv.WritePixel(canv.Width() - static_cast<int>(proj.m_coord.m_x), canv.Height() - static_cast<int>(proj.m_coord.m_y), Color(1.0f, 0.0f, 0.0f));
		}
	}

	canv.ExportAsPPM();
	system("RAYTRACER.ppm");
}

#endif