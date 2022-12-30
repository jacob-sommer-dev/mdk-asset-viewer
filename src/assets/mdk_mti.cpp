#include "mdk_mti.h"

int parsemti(const char *filename, MTI *mti)
{
    FILE *mtifile;

    if((mtifile = fopen(filename, "rb")) == NULL)
    {
        printf("mdk_mti: can't open %s\n", filename);
        return -1;
    }
    else
    {
        // allocate the fti and header. Set the header
        mti->header = (MTI_HDR*)malloc(sizeof(MTI_HDR));
        
        // for marking the position after reading the file length bytes
        int foff;

        // to make sure we're reading everything we should be
        size_t read;

        // read len of file
        read = fread(&(mti->header->length), sizeof(int), 1, mtifile);
        if(read != 1)
        {
            printf("mdk_mti: Couldn't read header: length\n");
            return -1;
        }
        // set offset, record data is offset from this
        foff = sizeof(int);
        
        // read file name
        read = fread(mti->header->title, sizeof(char), 12, mtifile);
        if(read != 12)
        {
            printf("mdk_mti: Couldn't read header: title\n");
            return -1;
        }
        mti->header->title[12] = '\0';

        // read data len
        read = fread(&(mti->header->foot_off), sizeof(int), 1, mtifile);
        if(read != 1)
        {
            printf("mdk_mti: Couldn't read header: foot_off\n");
            return -1;
        }

        // read the number of record entries
        read = fread(&(mti->header->num_entries), sizeof(int), 1, mtifile);
        if(read != 1)
        {
            printf("mdk_mti: Couldn't read header: number of entries\n");
            return -1;
        }

        // allocate enough memory for the record entries
        mti->records = (MTI_RECORD*)malloc(sizeof(MTI_RECORD) * mti->header->num_entries);

        // read in the record header data
        for(int i = 0; i < mti->header->num_entries; i++)
        {
            MTI_RECORD *record = (mti->records) + i;

            // -- title --
            read = fread(record->title, sizeof(char), 8, mtifile); // read 8 from the file
            if(read != 8)
            {
                printf("mdk_mti: Couldn't read record header %d of %d: title\n", i, mti->header->num_entries);
                return -1;
            }
            record->title[8] = '\0';

            // -- meta --
            read = fread(&(record->metaa), sizeof(short), 1, mtifile);
            if(read != 1)
            {
                printf("mdk_mti: Couldn't read record header %d of %d: metaa\n", i, mti->header->num_entries);
                return -1;
            }
            read = fread(&(record->metab), sizeof(short), 1, mtifile);
            if(read != 1)
            {
                printf("mdk_mti: Couldn't read record header %d of %d: metab\n", i, mti->header->num_entries);
                return -1;
            }
            read = fread(&(record->metac), sizeof(short), 1, mtifile);
            if(read != 1)
            {
                printf("mdk_mti: Couldn't read record header %d of %d: metac\n", i, mti->header->num_entries);
                return -1;
            }
            read = fread(&(record->metad), sizeof(short), 1, mtifile);
            if(read != 1)
            {
                printf("mdk_mti: Couldn't read record header %d of %d: metad\n", i, mti->header->num_entries);
                return -1;
            }
            read = fread(&(record->metae), sizeof(short), 1, mtifile);
            if(read != 1)
            {
                printf("mdk_mti: Couldn't read record header %d of %d: metae\n", i, mti->header->num_entries);
                return -1;
            }
            read = fread(&(record->metaf), sizeof(short), 1, mtifile);
            if(read != 1)
            {
                printf("mdk_mti: Couldn't read record header %d of %d: metaf\n", i, mti->header->num_entries);
                return -1;
            }

            // -- offset --
            read = fread(&(record->offset), sizeof(int), 1, mtifile);
            if(read != 1)
            {
                printf("mdk_mti: Couldn't read record header %d of %d: offset\n", i, mti->header->num_entries);
                return -1;
            }
        }

        // calculate lengths
        for(int i = 0; i < mti->header->num_entries; i++) 
        {
            MTI_RECORD *record = ((*mti).records) + i;

            // if the offset is 0, it's a color or something so maybe relevant info is in meta
            if (record->offset == 0)
            {
                record->len = 0;
                continue;
            }

            unsigned int start = record->offset + foff;
            unsigned int end = 0;

            // read to the next offset, or EOF if last record
            if(i == mti->header->num_entries - 1)
            {
                end = mti->header->foot_off;
            }
            else 
            {
                int idx = 1;
                while (end == 0 && idx < mti->header->num_entries - i)
                {
                    // next record
                    int nr = i + idx;
                    MTI_RECORD *nrecord = (mti->records) + nr;
                    unsigned int o = nrecord->offset;
                    if(o > 0)
                    {
                        end = o + foff;
                    }
                    else if (nr == mti->header->num_entries - 1)
                    {
                        end = mti->header->foot_off;
                    }
                    idx++;
                }
            }

            // calculate and set data length
            unsigned int len = end - start;
            record->len = len;
        }

        // read the actual data here
        for(int i = 0; i < mti->header->num_entries; i++) 
        {
            MTI_RECORD *record = ((*mti).records) + i;

            // if the offset/len is 0, it's a color or something so maybe relevant info is in meta
            if (record->len == 0)
            {
                continue;
            }

            unsigned int start = record->offset + foff;
            unsigned int len = record->len;

            // seek to the start of the data
            if(fseek(mtifile, (long)start, SEEK_SET) != 0)
            {
                printf("mdk_mti: Couldn't seek to record data %d of %d\n", i, mti->header->num_entries);
                return -1;
            }

            // allocate memory for data
            record->data = malloc(len);
            read = fread(record->data, 1, len, mtifile);
            if(read != len)
            {
                printf("mdk_mti: Couldn't read record data %d of %d\n", i, mti->header->num_entries);
                return -1;
            }
        }

        // close the file
        fclose(mtifile);

        return 0;
    }
}

int writemtifiles(MTI *mti, char *foldername)
{
    FILE *ofp;

    // write out to files
    for(int i =  0; i < mti->header->num_entries; i++) // fti->header->num_entries
    {
        MTI_RECORD *record = ((*mti).records) + i;

        if(record->len == 0)
        {
            //nothing to write
            continue;
        }

        char fname[255] = "";
        strncat(fname, foldername, 100);
        strncat(fname, "/", 60);

        printf("%s | o: %d | l: %d\n", record->title, record->offset, record->len);

        strncat(fname, record->title, 100);
        strncat(fname, ".bin", 5); // TODO file handling based on data in file (e.g. wav, .3dx, etc)

        // open the file with write permissions
        if((ofp = fopen(&fname[0], "w")) == NULL)
        {
            printf("mdk_mti: can't create file %s\n", fname);
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

void freemti(MTI *mti)
{
    // free record data
    for(int i = mti->header->num_entries - 1; i > -1; i--)
    {
        MTI_RECORD *record = ((*mti).records) + i;

        if(record->len == 0)
        {
            continue;
        }

        free(record->data);
        
    }

    // free record header
    free(mti->header);
    free(mti->records);
}