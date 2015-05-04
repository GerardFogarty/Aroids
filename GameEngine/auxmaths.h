#ifndef AUX_MATHS_H
#define AUX_MATHS_H

// file: auxmaths.h
// define utility maths functions
#include "graphics.h"

// rotate a 2D vector clockwise through an angle
VECTOR2 rotate(const VECTOR2& p, float angle);
VECTOR2 normalise(VECTOR2 v);

#endif