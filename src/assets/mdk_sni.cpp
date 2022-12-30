#include "mdk_sni.h"

int parsesni(char *filename, SNI *sni)
{
    FILE *snifile;

    if((snifile = fopen(filename, "rb")) == NULL)
    {
        printf("mdk_sni: can't open %s\n", filename);
        return -1;
    }
    else
    {
        // allocate the fti and header. Set the header
        sni->header = (SNI_HDR*)malloc(sizeof(SNI_HDR));
        
        // for marking the position after reading the file length bytes
        int foff;

        // to make sure we're reading everything we should be
        size_t read;

        // read len of file
        read = fread(&(sni->header->length), sizeof(int), 1, snifile);
        if(read != 1)
        {
            printf("mdk_sni: Couldn't read header: length\n");
            return -1;
        }
        // set offset, record data is offset from this
        foff = sizeof(int);
        
        // read file name
        read = fread(sni->header->title, sizeof(char), 12, snifile);
        if(read != 12)
        {
            printf("mdk_sni: Couldn't read header: title\n");
            return -1;
        }
        sni->header->title[12] = '\0';

        // read data len
        read = fread(&(sni->header->foot_off), sizeof(int), 1, snifile);
        if(read != 1)
        {
            printf("mdk_sni: Couldn't read header: foot_off\n");
            return -1;
        }

        // read the number of record entries
        read = fread(&(sni->header->num_entries), sizeof(int), 1, snifile);
        if(read != 1)
        {
            printf("mdk_sni: Couldn't read header: number of entries\n");
            return -1;
        }

        // allocate enough memory for the record entries
        sni->records = (SNI_RECORD*)malloc(sizeof(SNI_RECORD) * sni->header->num_entries);

        // read in the record header data
        for(int i = 0; i < sni->header->num_entries; i++)
        {
            SNI_RECORD *record = (sni->records) + i;

            // -- title --
            read = fread(record->title, sizeof(char), 12, snifile); // read 8 from the file
            if(read != 12)
            {
                printf("mdk_sni: Couldn't read record header %d of %d: title\n", i, sni->header->num_entries);
                return -1;
            }
            record->title[12] = '\0';

            // -- meta --
            read = fread(&(record->meta), sizeof(int), 1, snifile);
            if(read != 1)
            {
                printf("mdk_sni: Couldn't read record header %d of %d: meta\n", i, sni->header->num_entries);
                return -1;
            }

            // -- offset --
            read = fread(&(record->offset), sizeof(int), 1, snifile);
            if(read != 1)
            {
                printf("mdk_sni: Couldn't read record header %d of %d: offset\n", i, sni->header->num_entries);
                return -1;
            }

            // -- len --
            read = fread(&(record->len), sizeof(int), 1, snifile);
            if(read != 1)
            {
                printf("mdk_sni: Couldn't read record header %d of %d: len\n", i, sni->header->num_entries);
                return -1;
            }
        }

        // calculate lengths because it's FFFF sometimes
        for(int i = 0; i < sni->header->num_entries; i++) 
        {
            SNI_RECORD *record = ((*sni).records) + i;

            unsigned int start = record->offset + foff;
            unsigned int end;

            // read to the next offset, or EOF if last record
            if(i == sni->header->num_entries - 1)
            {
                end = sni->header->foot_off;
            }
            else 
            {
                // next record
                SNI_RECORD *nrecord = (sni->records) + (i + 1);
                unsigned int o = nrecord->offset;
                end = o + foff;
            }

            // calculate and set data length
            unsigned int len = end - start;
            record->len = len;
        }

        // read the actual data here
        for(int i = 0; i < sni->header->num_entries; i++) 
        {
            SNI_RECORD *record = ((*sni).records) + i;

            unsigned int start = record->offset + foff;
            unsigned int len = record->len;

            // seek to the start of the data
            if(fseek(snifile, (long)start, SEEK_SET) != 0)
            {
                printf("mdk_sni: Couldn't seek to record data %d of %d\n", i, sni->header->num_entries);
                return -1;
            }

            // allocate memory for data
            record->data = (int*)malloc(len);
            read = fread(record->data, 1, len, snifile);
            if(read != len)
            {
                printf("mdk_sni: Couldn't read record data %d of %d\n", i, sni->header->num_entries);
                return -1;
            }
        }

        // close the file
        fclose(snifile);

        return 0;
    }
}

int writesnifiles(SNI *sni, char *foldername)
{
    FILE *ofp;

    // write out to files
    for(int i =  0; i < sni->header->num_entries; i++) // fti->header->num_entries
    {
        char fname[255] = "";
        strncat(fname, foldername, 100);
        strncat(fname, "/", 60);

        SNI_RECORD *record = ((*sni).records) + i;

        printf("%s | o: %d | l: %d\n", record->title, record->offset, record->len);

        strncat(fname, record->title, 100);
        strncat(fname, ".bin", 5); // TODO file handling based on data in file (e.g. wav, .3dx, etc)

        // open the file with write permissions
        if((ofp = fopen(&fname[0], "w")) == NULL)
        {
            printf("mdk_sni: can't create file %s\n", fname);
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

void freesni(SNI *sni)
{
    // free record data
    for(int i = sni->header->num_entries - 1; i > -1; i--)
    {
        SNI_RECORD *record = ((*sni).records) + i;

        free(record->data);
    }

    // free record header
    free(sni->header);
    free(sni->records);
}