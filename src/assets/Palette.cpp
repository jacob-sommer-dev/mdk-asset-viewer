#include "Palette.hpp"

Palette::Palette()
{
    
}

void Palette::setData(void* data, unsigned int len)
{
    memcpy(this->data, data, len); // palettes are 768 bytes
}

void Palette::palette(const void*& palette)
{
    palette = this->data;
}

void Palette::colorAt(unsigned char idx, unsigned char* r, unsigned char* g, unsigned char* b)
{
    unsigned char *pp = (unsigned char*)data + sizeof(char) * idx*3;

    *r = *pp; // + sizeof(char) * 0;
	*g = *(pp + sizeof(char));// * 1;
	*b = *(pp + sizeof(char) * 2);
}