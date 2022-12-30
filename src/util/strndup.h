/*
Ref: https://opensource.apple.com/source/gcc/gcc-5575.11/libiberty/strndup.c
*/

#ifndef _STRNDUP_H_
#define _STRNDUP_H_

#include <string.h>
#include <stdlib.h>

char* strndup(const char* s, size_t n)
{
    char* res;
    size_t len = strlen(s);

    if(n < len)
    {
        len = n;
    }

    res = (char*)malloc(len + 1);
    if(!res)
    {
        return 0;
    }

    res[len] = '\0';
    return (char*)memcpy(res, s, len);
}


#endif //_STRNDUP_H_