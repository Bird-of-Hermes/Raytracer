#include "../SourceHeaders/Filters.h"
//	 0123456789
//0  xxxxxxxxxx
//1  xxxxxoxxxx
//2  xxxxxxxxxx

// 1,1 to fullheight-1, fullwidth-1

const void MedianFilter(Canvas* canv, uint32_t kernel, Canvas* ptr)
{
	// iterators
	std::vector<uint32_t>HeightIter(canv->Height());
	std::vector<uint32_t>WidthIter(canv->Width());
	for (int32_t i = 0; i < canv->Height(); ++i)
	{
		HeightIter[i] = i;
	}
	for (int32_t i = 0; i < canv->Width(); i++)
	{
		WidthIter[i] = i;
	}
	Canvas exported(canv->Width(),canv->Height());

	const int32_t halfkernel = kernel / 2;

	std::for_each(std::execution::par, HeightIter.begin(), HeightIter.end(), [&](int32_t y)
		{
			std::for_each(std::execution::par, WidthIter.begin(), WidthIter.end(), [&, y](int32_t x)
				{
					if (y < halfkernel || y >= canv->Height()-halfkernel)
						return;
					if (x < halfkernel || x >= canv->Width()-halfkernel)
						return;
					
					std::vector<float>ArrayRed;
					std::vector<float>ArrayGreen;
					std::vector<float>ArrayBlue;
					for (int i = -halfkernel; i <= halfkernel; i++)
					{
						for (int j = -halfkernel; j <= halfkernel; j++)
						{
							ArrayRed.push_back(canv->PixelAt(y + i, x + j).r);
							ArrayGreen.push_back(canv->PixelAt(y + i, x + j).g);
							ArrayBlue.push_back(canv->PixelAt(y + i, x + j).b);
						}
					}
					std::sort(ArrayRed.begin(), ArrayRed.end());
					std::sort(ArrayGreen.begin(), ArrayGreen.end());
					std::sort(ArrayBlue.begin(), ArrayBlue.end());
					
					Color medianChannelRed = {ArrayRed[(kernel*kernel)/2], 0.0f, 0.0f};
					Color medianChannelGreen = { 0.0f, ArrayGreen[(kernel*kernel) / 2], 0.0f };
					Color medianChannelBlue = { 0.0f, 0.0f, ArrayBlue[(kernel*kernel) / 2] };

					Color medianFinal = medianChannelRed + medianChannelGreen + medianChannelBlue;
					exported.WritePixel(y, x, medianFinal);
				});
		});
	exported.ExportAsPPM("Files/MF_RAYTRACER.ppm");
	if (ptr != nullptr)
		ptr->DataCopy(exported);
}
const void AAFilter(Canvas* canv, Canvas* maskimg)
{
	// iterators
	std::vector<uint32_t>HeightIter(canv->Height());
	std::vector<uint32_t>WidthIter(canv->Width());
	for (int32_t i = 0; i < canv->Height(); ++i)
	{
		HeightIter[i] = i;
	}
	for (int32_t i = 0; i < canv->Width(); i++)
	{
		WidthIter[i] = i;
	}

	std::for_each(std::execution::par, HeightIter.begin(), HeightIter.end(), [&](int32_t y)
		{
			std::for_each(std::execution::par, WidthIter.begin(), WidthIter.end(), [&, y](int32_t x)
				{
					if (maskimg->PixelAt(y, x) != BLACK)
						return;

					int xPrev = std::max(x - 1, 0);
					int xNext = std::min(x + 1, canv->Width() - 1);
					int yPrev = std::max(y - 1, 0);
					int yNext = std::min(y + 1, canv->Height() - 1);

					Color prevPixel = canv->PixelAt(y, xPrev);
					Color nextPixel = canv->PixelAt(y, xNext);			
					Color abovePixel = canv->PixelAt(yPrev, x);
					Color belowPixel = canv->PixelAt(yNext, x);
					Color northwestPixel = canv->PixelAt(yPrev, xPrev);
					Color northeastPixel = canv->PixelAt(yPrev, xNext);
					Color southwestPixel = canv->PixelAt(yNext, xPrev);
					Color southeastPixel = canv->PixelAt(yNext, xNext);
					
					// Average
					Color smoothedPixel;
					smoothedPixel.r = (prevPixel.r + nextPixel.r + abovePixel.r + belowPixel.r + northwestPixel.r + northeastPixel.r + southwestPixel.r + southeastPixel.r+ canv->PixelAt(y, x).r) / 9.0f;
					smoothedPixel.g = (prevPixel.g + nextPixel.g + abovePixel.g + belowPixel.g + northwestPixel.g + northeastPixel.g + southwestPixel.g + southeastPixel.g + canv->PixelAt(y, x).g) / 9.0f;
					smoothedPixel.b = (prevPixel.b + nextPixel.b + abovePixel.b + belowPixel.b + northwestPixel.b + northeastPixel.b + southwestPixel.b + southeastPixel.b + canv->PixelAt(y, x).b) / 9.0f;

					canv->WritePixel(y, x, smoothedPixel);
				});
		});
	canv->ExportAsPPM("Files/AA_RAYTRACER.ppm");
}
const void SobelOperator(Canvas* canv, Canvas* out)
{
	// iterators
	std::vector<uint32_t>HeightIter(canv->Height());
	std::vector<uint32_t>WidthIter(canv->Width());
	for (int32_t i = 0; i < canv->Height(); ++i)
	{
		HeightIter[i] = i;
	}
	for (int32_t i = 0; i < canv->Width(); i++)
	{
		WidthIter[i] = i;
	}
	Canvas edgedetector(canv->Width(), canv->Height());
	Canvas finalimg(canv->Width(), canv->Height());

	// first we grayscale the canvas
	std::for_each(std::execution::par, HeightIter.begin(), HeightIter.end(), [&](int32_t y)
		{
			std::for_each(std::execution::par, WidthIter.begin(), WidthIter.end(), [&, y](int32_t x)
				{
					// 0.299083 0.585841 0.114076 SDTV
					Color rgb = canv->PixelAt(y, x);
					Color grayscale = { rgb.r * 0.299083f, rgb.g * 0.585941f, rgb.b * 0.114076f };
					edgedetector.WritePixel(y, x, grayscale);
				});
		});

	// then we apply the sobel operator
	std::for_each(std::execution::par, HeightIter.begin(), HeightIter.end(), [&](int32_t y)
		{
			std::for_each(std::execution::par, WidthIter.begin(), WidthIter.end(), [&, y](int32_t x)
				{
					//  x
					// -1 0 1
					// -2 0 2
					// -1 0 1

					//  y
					// -1 -2 -1
					//  0  0  0
					//  1  2  1

					int xPrev = std::max(x - 1, 0);
					int xNext = std::min(x + 1, edgedetector.Width() - 1);
					int yPrev = std::max(y - 1, 0);
					int yNext = std::min(y + 1, edgedetector.Height() - 1);

					Color prevPixel = edgedetector.PixelAt(y, xPrev);
					Color nextPixel = edgedetector.PixelAt(y, xNext);
					Color abovePixel = edgedetector.PixelAt(yPrev, x);
					Color belowPixel = edgedetector.PixelAt(yNext, x);
					Color northwestPixel = edgedetector.PixelAt(yPrev, xPrev);
					Color northeastPixel = edgedetector.PixelAt(yPrev, xNext);
					Color southwestPixel = edgedetector.PixelAt(yNext, xPrev);
					Color southeastPixel = edgedetector.PixelAt(yNext, xNext);

					Color edgeX =
					{
						northwestPixel * -1.0f +              northeastPixel +
						prevPixel * -2.0f +                   nextPixel * 2.0f +
						southwestPixel * -1.0f +                southeastPixel
					};
					Color edgeY
					{
						northwestPixel * -1.0f + abovePixel * -2.0f + northeastPixel * -1.0f +
						southwestPixel +         belowPixel * 2.0f  + southeastPixel
					};
					float gradient = std::sqrtf(edgeX.r*edgeX.r + edgeX.g*edgeX.g + edgeX.b*edgeX.b + edgeY.r*edgeY.r + edgeY.g*edgeY.g + edgeY.b*edgeY.b);
					if (edgedetector.PixelAt(y, x).r + edgedetector.PixelAt(y, x).g + edgedetector.PixelAt(y, x).b > gradient)
					{
						finalimg.WritePixel(y, x, WHITE);
					}
					else
						finalimg.WritePixel(y, x, BLACK);
				});
		});
	finalimg.ExportAsPPM("Files/SO_RAYTRACER.ppm");
	if(out != nullptr)
		out->DataCopy(finalimg);
}

const void BlendMultiply(Canvas* canv)
{
	// iterators
	std::vector<uint32_t>HeightIter(canv->Height());
	std::vector<uint32_t>WidthIter(canv->Width());
	for (int32_t i = 0; i < canv->Height(); ++i)
	{
		HeightIter[i] = i;
	}
	for (int32_t i = 0; i < canv->Width(); i++)
	{
		WidthIter[i] = i;
	}

	std::for_each(std::execution::par, HeightIter.begin(), HeightIter.end(), [&](int32_t y)
		{
			std::for_each(std::execution::par, WidthIter.begin(), WidthIter.end(), [&, y](int32_t x)
				{
					// Multiply
					Color blendedPixel = canv->PixelAt(y, x) * canv->PixelAt(y, x);

					canv->WritePixel(y, x, blendedPixel);
				});
		});
	canv->ExportAsPPM("Files/BlendedMultiply_RAYTRACER.ppm");
}
