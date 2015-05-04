#include <cmath>

#include "Maths.h"

float distance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    return ::sqrtf( dx * dx + dy * dy );
}