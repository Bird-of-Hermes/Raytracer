#ifndef CANVAS_H
#define CANVAS_H
#include "Tuples.h"
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
	void WritePixel(uint32_t x, uint32_t y, Color color) { this->cor[x][y].r = color.r; this->cor[x][y].g = color.g; this->cor[x][y].b = color.b; }
	void ExportAsPPM()
	{
		std::ofstream fon("RAYTRACER.ppm", std::ios::binary);

		if (!fon.is_open())
			std::cout << "Couldn't write file\n";

		fon << "P6\n";
		fon << width << ' ' << height << '\n';
		fon << "255\n";

		for (int32_t i = 0; i < width; i++)
		{
			for (int32_t j = 0; j < height; j++)
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
		cor = new Color * [width];

		for (int32_t i = 0; i < width; i++)
		{
			cor[i] = new Color[height];
		}
		Color ff {};

		int max_threads = omp_get_max_threads();

		auto start = std::chrono::high_resolution_clock::now();
		
		#pragma omp parallel for num_threads(max_threads)

		for (int32_t i = 0; i < width; i++)
		{
			for (int32_t j = 0; j + 1 < height; j += 12)
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
		for (int32_t i = 0; i < width; i++)
		{
			delete[] cor[i];
		}
		delete[] cor;
	}

};
#endif