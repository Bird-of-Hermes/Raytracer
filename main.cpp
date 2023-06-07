#include "Canvas.h"
#include "Matrixes.h"

int main()
{
	Canvas Clock(600, 600);

	Tuple::Pos Hours[12]
	{
		Tuple::Point(0.0f, 1.0f, 0.0f),  // 12
		Tuple::Point(0.0f, 1.0f, 0.0f),  // 11
		Tuple::Point(0.0f, 1.0f, 0.0f),  // 10
		Tuple::Point(0.0f, 1.0f, 0.0f),  // 9
		Tuple::Point(0.0f, 1.0f, 0.0f),	 // 8
		Tuple::Point(0.0f, 1.0f, 0.0f),	 // 7
		Tuple::Point(0.0f, 1.0f, 0.0f),	 // 6
		Tuple::Point(0.0f, 1.0f, 0.0f),	 // 5
		Tuple::Point(0.0f, 1.0f, 0.0f),	 // 4
		Tuple::Point(0.0f, 1.0f, 0.0f),	 // 3
		Tuple::Point(0.0f, 1.0f, 0.0f),	 // 2
		Tuple::Point(0.0f, 1.0f, 0.0f)   // 1
	};

	// Rotate points
	for (int i = 0; i < 12; i++)
		Hours[i] = RotateZaxis(30.0f * i) * Hours[i];

	// Translate and scale points
	const float scaleFactor = 0.5f;
	const float xOffset = Clock.Width() * 0.5f;
	const float yOffset = Clock.Height() * 0.5f;

	int32_t x[12]{};
	int32_t y[12]{};

	for (int i = 0; i < 12; i++)
	{
		x[i] = static_cast<int32_t>(scaleFactor * (Hours[i].m_x * xOffset) + xOffset);
		y[i] = static_cast<int32_t>(scaleFactor * (Hours[i].m_y * yOffset) + yOffset);
	}

	// Draw points
	for (int i = 0; i < 12; i++)
		Clock.WritePixel(static_cast<int>(x[i]), static_cast<int>(y[i]), Color(1.0f, 1.0f, 1.0f));

	Clock.ExportAsPPM();

	return 0;
}