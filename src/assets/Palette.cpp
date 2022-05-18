#include "Palette.hpp"

Palette::Palette(void* data)
{
    memcpy(this->data, data, 768); // palettes are 768 bytes
}

void Palette::palette(const void* palette)
{
    palette = this->data;
}