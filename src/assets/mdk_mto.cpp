#include "mdk_mto.h"

int parsemto(char *filename, MTO* mto)
{
    FILE *mtofile;

    if((mtofile = fopen(filename, "rb")) == NULL)
    {
        printf("mdk_mto: can't open %s\n", filename);
        return -1;
    }
    else
    {
        // allocate the mto and header. Set the header
        mto->header = (MTO_HDR*)malloc(sizeof(MTO_HDR));

        // to make sure we're reading everything we should be
        size_t read;

        // read len of file
        read = fread(&(mto->header->length), sizeof(int), 1, mtofile);
        if(read != 1)
        {
            printf("mdk_mto: Couldn't read header: length\n");
            return -1;
        }
        
        // read file name
        read = fread(mto->header->title, sizeof(char), 12, mtofile);
        if(read != 12)
        {
            printf("mdk_mto: Couldn't read header: title\n");
            return -1;
        }
        mto->header->title[12] = '\0';

        // read data len
        read = fread(&(mto->header->foot_off), sizeof(int), 1, mtofile);
        if(read != 1)
        {
            printf("mdk_mto: Couldn't read header: foot_off\n");
            return -1;
        }

        // read the number of record entries
        read = fread(&(mto->header->num_entries), sizeof(int), 1, mtofile);
        if(read != 1)
        {
            printf("mdk_mto: Couldn't read header: number of entries\n");
            return -1;
        }

        // allocate enough memory for the record entries
        mto->records = (MTO_RECORD*)malloc(sizeof(MTO_RECORD) * mto->header->num_entries);

        // read in the record header data
        for(int i = 0; i < mto->header->num_entries; i++)
        {
            MTO_RECORD *record = (mto->records) + i;

            // -- title --
            read = fread(record->title, sizeof(char), 8, mtofile); // read 8 from the file
            if(read != 8)
            {
                printf("mdk_mto: Couldn't read record header %d of %d: title\n", i, mto->header->num_entries);
                return -1;
            }
            record->title[8] = '\0';

            // -- offset --
            read = fread(&(record->offset), sizeof(int), 1, mtofile);
            if(read != 1)
            {
                printf("mdk_mto: Couldn't read record header %d of %d: offset\n", i, mto->header->num_entries);
                return -1;
            }

            // we need all the record header data to pull the actual data, so don't get the data now
        }

        // read the actual data here
        for(int i = 0; i < mto->header->num_entries; i++) 
        {
            MTO_RECORD *record = (mto->records) + i;

            unsigned int start = record->offset;
            unsigned int end;

            // read to the next offset, or EOF if last record
            if(i == mto->header->num_entries - 1)
            {
                end = mto->header->foot_off;
            }
            else 
            {
                // next record
                MTO_RECORD *nrecord = (mto->records) + (i + 1);
                unsigned int o = nrecord->offset;
                end = o;
            }

            // calculate and set data length
            unsigned int len = end - start;
            record->len = len;
        }

        for (int i = 0; i < mto->header->num_entries; i++)
        {
            MTO_RECORD *record = ((*mto).records) + i;

            unsigned int start = record->offset;
            unsigned int len = record->len;

            // seek to the start of the data
            if(fseek(mtofile, (long)start, SEEK_SET) != 0)
            {
                printf("mdk_mto: Couldn't seek to record data %d of %d\n", i, mto->header->num_entries);
                return -1;
            }

            // allocate memory for data
            record->data = (int*)malloc(len);
            read = fread(record->data, 1, len, mtofile);
            if(read != len)
            {
                printf("mdk_mto: Couldn't read record data %d of %d\n", i, mto->header->num_entries);
                return -1;
            }
        }

        // close the file
        fclose(mtofile);

        return 0;
    }
}

int writemtofiles(MTO *mto, char *foldername)
{
    FILE *ofp;

    // write out to files
    for(int i =  0; i < mto->header->num_entries; i++) // mto->header->num_entries
    {
        char fname[255] = "";
        strncat(fname, foldername, 100);
        strncat(fname, "/", 60);

        MTO_RECORD *record = ((*mto).records) + i;

        printf("%s | o: %d | l: %d\n", record->title, record->offset, record->len);

        strncat(fname, record->title, 8);
        strncat(fname, ".bin", 5); // TODO file handling based on data in file (e.g. wav, .3dx, etc)

        // open the file with write permissions
        if((ofp = fopen(&fname[0], "w")) == NULL)
        {
            printf("mdk_mto: can't create file %s\n", fname);
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

void freemto(MTO *mto)
{
    // free record data
    for(int i = mto->header->num_entries - 1; i > -1; i--)
    {
        MTO_RECORD *record = ((*mto).records) + i;

        free(record->data);
    }

    // free record header
    free(mto->header);
    free(mto->records);
}