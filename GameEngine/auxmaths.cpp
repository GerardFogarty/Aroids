#include "auxmaths.h"

// rotate a 2D vector clockwise through an angle
VECTOR2 rotate(const VECTOR2& p, float angle)
{
    VECTOR2 vOut(
		p.x * cos(angle) - p.y * sin(angle),
		p.x * sin(angle) + p.y * cos(angle));
    return vOut;
}

VECTOR2 normalise(VECTOR2 v)
{
    float magnitude = (float) sqrt(v.x * v.x + v.y * v.y);
    if (magnitude == 0.0)
        magnitude = 0.000001;

    v.x /= magnitude;
    v.y /= magnitude;

	return v;
}