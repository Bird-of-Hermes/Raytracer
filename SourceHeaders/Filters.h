#ifndef FILTERS_H
#define FILTERS_H

#include "Canvas.h"
#include <vector>
#include <execution>
#include <algorithm>

const void MedianFilter(Canvas* canv, uint32_t kernel, Canvas* ptr = nullptr);
const void AAFilter(Canvas* canv, Canvas* maskimg);
const void SobelOperator(Canvas* canv, Canvas* out = nullptr);
const void BlendMultiply(Canvas* canv);
#endif