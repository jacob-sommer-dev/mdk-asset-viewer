#ifndef __PALETTE_H__
#define __PALLETE_H__

#include <string.h>

class Palette
{

private:
    unsigned char data[768];

public:
    Palette();

    void setData(void*, unsigned int);

    void palette(const void*&);
};

#endif // __PALETTE_H__