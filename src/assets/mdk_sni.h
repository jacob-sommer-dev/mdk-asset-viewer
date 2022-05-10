#ifndef __MDK_SNI_H__
#define __MDK_SNI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _sni_hdr {
    unsigned int length;        // length of the file (file len - these 4 bytes)
    char title[13];             // title for the file
    unsigned int foot_off;      // data length (up to footer, just file title again)
    unsigned int num_entries;   // number of record entries
} SNI_HDR;

typedef struct _sni_record {
    char title[13];             // title of record
    unsigned int meta;          // unknown metadata
    unsigned int offset;        // offset from file start - 4 file length bytes
    unsigned int len;           // length of the data
    int *data;                  // the data
} SNI_RECORD;

typedef struct _sni {
    SNI_HDR *header;
    SNI_RECORD *records;
} SNI;

int parsesni(char *, SNI *);
int writesnifiles(SNI *, char *);
void freesni(SNI *);

#ifdef __cplusplus
}
#endif

#endif