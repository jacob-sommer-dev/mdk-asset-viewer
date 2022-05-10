#include "mdk_fti.h"

int parsefti(char *filename, FTI* fti)
{
    FILE *ftifile;

    if((ftifile = fopen(filename, "r")) == NULL)
    {
        printf("mdk_fti: can't open %s\n", filename);
        return -1;
    }
    else
    {
        // allocate the fti and header. Set the header
        fti->header = (FTI_HDR*)malloc(sizeof(FTI_HDR));
        
        // for marking the position after reading the file length bytes
        int foff;

        // to make sure we're reading everything we should be
        size_t read;

        // read len of file
        read = fread(&(fti->header->length), __SIZEOF_INT__, 1, ftifile);
        if(read != 1)
        {
            printf("mdk_fti: Couldn't read header: length\n");
            return -1;
        }
        // set offset, record data is offset from this
        foff = __SIZEOF_INT__;

        // read the number of record entries
        read = fread(&(fti->header->num_entries), __SIZEOF_INT__, 1, ftifile);
        if(read != 1)
        {
            printf("mdk_fti: Couldn't read header: number of entries\n");
            return -1;
        }

        // allocate enough memory for the record entries
        fti->records = (FTI_RECORD*)malloc(sizeof(FTI_RECORD) * fti->header->num_entries);

        // read in the record header data
        for(int i = 0; i < fti->header->num_entries; i++)
        {
            FTI_RECORD *record = (fti->records) + i;

            // -- title --
            read = fread(record->title, sizeof(char), 8, ftifile); // read 8 from the file
            if(read != 8)
            {
                printf("mdk_fti: Couldn't read record header %d of %d: title\n", i, fti->header->num_entries);
                return -1;
            }
            record->title[8] = '\0';

            // -- offset --
            read = fread(&(record->offset), __SIZEOF_INT__, 1, ftifile);
            if(read != 1)
            {
                printf("mdk_fti: Couldn't read record header %d of %d: offset\n", i, fti->header->num_entries);
                return -1;
            }

            // we need all the record header data to pull the actual data, so don't get the data now
        }

        // read the actual data here
        for(int i = 0; i < fti->header->num_entries; i++) 
        {
            FTI_RECORD *record = (fti->records) + i;

            unsigned int start = record->offset + foff;
            unsigned int end;

            // read to the next offset, or EOF if last record
            if(i == fti->header->num_entries - 1)
            {
                end = fti->header->length + foff;
            }
            else 
            {
                // next record
                FTI_RECORD *nrecord = (fti->records) + (i + 1);
                unsigned int o = nrecord->offset;
                end = o + foff;
            }

            // calculate and set data length
            unsigned int len = end - start;
            record->len = len;
        }

        for (int i = 0; i < fti->header->num_entries; i++)
        {
            FTI_RECORD *record = ((*fti).records) + i;

            unsigned int start = record->offset + foff;
            unsigned int len = record->len;

            // seek to the start of the data
            if(fseek(ftifile, (long)start, SEEK_SET) != 0)
            {
                printf("mdk_fti: Couldn't seek to record data %d of %d\n", i, fti->header->num_entries);
                return -1;
            }

            // allocate memory for data
            record->data = (int*)malloc(len);
            read = fread(record->data, 1, len, ftifile);
            if(read != len)
            {
                printf("mdk_fti: Couldn't read record data %d of %d\n", i, fti->header->num_entries);
                return -1;
            }
        }

        // close the file
        fclose(ftifile);

        return 0;
    }
}

int writeftifiles(FTI *fti, char *foldername)
{
    FILE *ofp;

    // write out to files
    for(int i =  0; i < fti->header->num_entries; i++) // fti->header->num_entries
    {
        char fname[255] = "";
        strncat(fname, foldername, 100);
        strncat(fname, "/", 60);

        FTI_RECORD *record = ((*fti).records) + i;

        printf("%s | o: %d | l: %d\n", record->title, record->offset, record->len);

        strncat(fname, record->title, 8);
        strncat(fname, ".bin", 5); // TODO file handling based on data in file (e.g. wav, .3dx, etc)

        // open the file with write permissions
        if((ofp = fopen(&fname[0], "w")) == NULL)
        {
            printf("mdk_fti: can't create file %s\n", fname);
            return 1;
        }
        else 
        {
            // write the data
            fwrite(record->data, 1, record->len, ofp);

            // close the file
            fclose(ofp);
        }
    }
    printf("\n");
    return 0;
}

void freefti(FTI *fti)
{
    // free record data
    for(int i = fti->header->num_entries - 1; i > -1; i--)
    {
        FTI_RECORD *record = ((*fti).records) + i;

        free(record->data);
    }

    // free record header
    free(fti->header);
    free(fti->records);
}