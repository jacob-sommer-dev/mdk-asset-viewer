#include "Brush.hpp"

Brush::Brush(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    type = BRUSH;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
}

