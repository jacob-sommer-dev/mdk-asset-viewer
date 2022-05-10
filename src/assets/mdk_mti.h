#ifndef __MDK_MTI_H__
#define __MDK_MTI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _mti_hdr {
    unsigned int length;        // length of the file (file len - these 4 bytes)
    char title[13];             // title for the file
    unsigned int foot_off;      // data length (up to footer, just file title again)
    unsigned int num_entries;   // number of record entries
} MTI_HDR;

typedef struct _mti_record {
    char title[9];              // title of record
    unsigned short metaa;        // unknown metadata
    unsigned short metab;        // unknown metadata
    unsigned short metac;        // unknown metadata
    unsigned short metad;        // unknown metadata
    unsigned short metae;        // unknown metadata
    unsigned short metaf;        // unknown metadata
    unsigned int offset;        // offset from file start - 4 file length bytes
    unsigned int len;           // length of the data
    int *data;                  // the data
} MTI_RECORD;

typedef struct _mti {
    MTI_HDR *header;
    MTI_RECORD *records;
} MTI;

int parsemti(char *, MTI *);
int writemtifiles(MTI *, char *);
void freemti(MTI *);

#ifdef __cplusplus
}
#endif

#endif