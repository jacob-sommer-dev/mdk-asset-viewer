#ifndef __MDK_BNI_H__
#define __MDK_BNI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _bni_hdr {
    unsigned int length;        // length of the file (file len - these 4 bytes)
    unsigned int num_entries;   // number of record entries
} BNI_HDR;

typedef struct _bni_record {
    char title[13];             // title of record
    unsigned int offset;        // offset from file start - 4 file length bytes
    unsigned int len;           // length of the data
    void *data;                 // the data
} BNI_RECORD;

typedef struct _bni {
    BNI_HDR *header;
    BNI_RECORD *records;
} BNI;

int parsebni(const char *, BNI *);
// int writebnifiles(BNI *, char *);
void freebni(BNI *);

#ifdef __cplusplus
}
#endif

#endif