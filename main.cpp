#include "Canvas.h"
#include "Projectile.h"

int main()
{
	Canvas canv(900, 600);

	Projectile proj(Tuple::Point(1.0f, 1.0f, 0.0f), Tuple::Normalize(Tuple::Pos(1.0f, 1.8f, 0.0f)) * 11.25f);
	Environment env(Tuple::Pos(0.01f, -0.1f, 0.0f), Tuple::Pos(-0.02f, 0.011f, 0.0f));

	for (size_t i = 0; i < canv.Width(); i++)
	{
		for (size_t j = 0; j < canv.Height(); j++)
		{
			proj = tick(env, proj);
			if (proj.m_coord.m_y >= canv.Height() || proj.m_coord.m_y < 0 || proj.m_coord.m_x >= canv.Width() || proj.m_coord.m_x < 0)
				continue;
			else
				canv.WritePixel(canv.Width() - proj.m_coord.m_x, canv.Height() - proj.m_coord.m_y, Color(1.0f, 0.0f, 0.0f));
		}
	}

	canv.ExportAsPPM();
	system("RAYTRACER.ppm");

	return 0;
}