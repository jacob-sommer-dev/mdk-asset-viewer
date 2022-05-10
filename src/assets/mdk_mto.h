#ifndef __MDK_MTO_H__
#define __MDI_MTO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _mto_hdr {
    unsigned int length;        // length of the file (file len - these 4 bytes)
    char title[13];             // title for the file
    unsigned int foot_off;      // data length (up to footer, just file title again)
    unsigned int num_entries;   // number of record entries
} MTO_HDR;

typedef struct _mto_record {
    char title[9];              // title of record
    unsigned int offset;        // offset from file start
    unsigned int len;           // calculated length of the data
    int *data;                  // the data
} MTO_RECORD;

typedef struct _mto {
    MTO_HDR *header;            // the header
    MTO_RECORD *records;        // the records
} MTO;

int parsemto(char *, MTO *);
int writemtofiles(MTO *, char *);
void freemto(MTO *);

#ifdef __cplusplus
}
#endif

#endif