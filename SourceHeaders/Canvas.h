#ifndef CANVAS_H
#define CANVAS_H
#include "Tuples.h"
#include "Projectile.h"
#include <chrono>
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
	inline const void WritePixel(int32_t y, int32_t x, Color color)
	{
		if (y >= Height() || y < 0 || x >= Width() || x < 0)
		{
			std::cerr << "y > Canva's Height or x > Canva's Width\n";
			exit(EXIT_FAILURE);
		}

		if (color.r > 1.0f)
			color.r = 1.0f;
		if (color.g > 1.0f)
			color.g = 1.0f;
		if (color.b > 1.0f)
			color.b = 1.0f;

		cor[y][x].r = color.r;
		cor[y][x].g = color.g;
		cor[y][x].b = color.b;
	}


	void ExportAsPPM(std::string filepath)
	{
		std::ofstream fon(filepath, std::ios::binary);

		if (!fon.is_open())
		{
			std::cerr << "Couldn't write to file\n";
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
	void DataCopy(const Canvas& other)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				cor[i][j] = other.cor[i][j];
			}
		}
	}

	explicit Canvas(uint32_t w, uint32_t h) : width(w), height(h)
	{
		cor = new Color * [height];

		for (int32_t i = 0; i < height; i++)
			cor[i] = new Color[width];
	}
	~Canvas()
	{
		for (int32_t i = 0; i < height; i++)
		{
			if(cor[i] != nullptr)
				delete cor[i];
		}
		delete[] cor;
	}
};

inline void CanvasTest()
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

	canv.ExportAsPPM("Files/RAYTRACER.ppm");
	system("Files\\RAYTRACER.ppm");
}

#endif