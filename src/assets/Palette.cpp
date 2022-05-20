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