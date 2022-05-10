#ifndef __MDK_FTI_H__
#define __MDI_FTI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _fti_hdr {
    unsigned int length;        // length of the file (file len - these 4 bytes)
    unsigned int num_entries;   // number of record entries
} FTI_HDR;

typedef struct _fti_record {
    char title[9];              // title of record
    unsigned int offset;        // offset from file start - 4 file length bytes
    unsigned int len;           // calculated length of the data
    int *data;                  // the data
} FTI_RECORD;

typedef struct _fti {
    FTI_HDR *header;            // the header
    FTI_RECORD *records;        // the records
} FTI;

int parsefti(char *, FTI *);
int writeftifiles(FTI *, char *);
void freefti(FTI *);

#ifdef __cplusplus
}
#endif

#endif