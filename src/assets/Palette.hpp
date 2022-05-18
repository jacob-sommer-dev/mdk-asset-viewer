#ifndef __PALETTE_H__
#define __PALLETE_H__

#include <string.h>

class Palette
{

private:
    void* data;

public:
    Palette(void* data);
    ~Palette();

    void palette(const void*);
};

#endif // __PALETTE_H__