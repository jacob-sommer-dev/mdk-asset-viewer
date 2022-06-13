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
    void colorAt(unsigned char, unsigned char*, unsigned char*, unsigned char*);
};

#endif // __PALETTE_H__