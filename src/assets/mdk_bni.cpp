#include "mdk_bni.h"

int parsebni(const char *filename, BNI* bni)
{
    FILE *bnifile;

    if((bnifile = fopen(filename, "rb")) == NULL)
    {
        printf("mdk_bni: can't open %s\n", filename);
        return -1;
    }
    else
    {
        // allocate the bni and header. Set the header
        bni->header = (BNI_HDR*)malloc(sizeof(BNI_HDR));

        // to make sure we're reading everything we should be
        size_t read;

        // read len of file
        read = fread(&(bni->header->length), sizeof(int), 1, bnifile);
        if(read != 1)
        {
            printf("mdk_bni: Couldn't read header: length\n");
            return -1;
        }

        // read the number of record entries
        read = fread(&(bni->header->num_entries), sizeof(int), 1, bnifile);
        if(read != 1)
        {
            printf("mdk_bni: Couldn't read header: number of entries\n");
            return -1;
        }

        // allocate enough memory for the record entries
        bni->records = (BNI_RECORD*)malloc(sizeof(BNI_RECORD) * bni->header->num_entries);

        // read in the record header data
        for(int i = 0; i < bni->header->num_entries; i++)
        {
            BNI_RECORD *record = (bni->records) + i;

            // -- title --
            read = fread(record->title, sizeof(char), 12, bnifile); // read 12 from the file
            if(read != 12)
            {
                printf("mdk_bni: Couldn't read record header %d of %d: title\n", i+1, bni->header->num_entries);
                return -1;
            }
            record->title[13] = '\0';

            // -- offset --
            read = fread(&(record->offset), sizeof(int), 1, bnifile);
            if(read != 1)
            {
                printf("mdk_bni: Couldn't read record header %d of %d: offset\n", i+1, bni->header->num_entries);
                // if(ferror(bnifile)) 
                // {
                //     printf("error");
                // } 
                // else if(feof(bnifile))
                // {
                //     printf("eof");
                // }
                return -1;
            }

            // we need all the record header data to pull the actual data, so don't get the data now
        }

        // read the actual data here
        for(int i = 0; i < bni->header->num_entries; i++) 
        {
            BNI_RECORD *record = (bni->records) + i;

            unsigned int start = record->offset;
            unsigned int end = 0;

            // read to the next offset, or EOF if last record
            if(i == bni->header->num_entries - 1)
            {
                end = bni->header->length; // total length
            }
            else 
            {
                // next record
                BNI_RECORD *nrecord = (bni->records) + i + 1;
                end = nrecord->offset;
            }

            // calculate and set data length
            unsigned int len = end - start;
            record->len = len;
        }

        for (int i = 0; i < bni->header->num_entries; i++)
        {
            BNI_RECORD *record = ((*bni).records) + i;

            unsigned int start = record->offset + 4;
            unsigned int len = record->len;

            // seek to the start of the data
            if(fseek(bnifile, (long)start, SEEK_SET) != 0)
            {
                printf("mdk_bni: Couldn't seek to record data %d of %d\n", i+1, bni->header->num_entries);
                return -1;
            }

            // allocate memory for data
            record->data = (int*)malloc(len);
            read = fread(record->data, 1, len, bnifile);
            if(read != len)
            {
                printf("mdk_bni: Couldn't read record data %d of %d\n", i+1, bni->header->num_entries);
                if(ferror(bnifile)) 
                {
                    printf("error");
                } 
                else if(feof(bnifile))
                {
                    printf("eof");
                }
                return -1;
            }
        }

        // close the file
        fclose(bnifile);

        return 0;
    }
}

// int writebnifiles(BNI *bni, char *foldername)
// {
//     FILE *ofp;

//     // write out to files
//     for(int i =  0; i < bni->header->num_entries; i++) // bni->header->num_entries
//     {
//         char fname[255] = "";
//         strncat(fname, foldername, 100);
//         strncat(fname, "/", 60);

//         BNI_RECORD *record = ((*bni).records) + i;

//         printf("%s | o: %d | l: %d\n", record->title, record->offset, record->len);

//         strncat(fname, record->title, 8);
//         strncat(fname, ".bin", 5); // TODO file handling based on data in file (e.g. wav, .3dx, etc)

//         // open the file with write permissions
//         if((ofp = fopen(&fname[0], "w")) == NULL)
//         {
//             printf("mdk_bni: can't create file %s\n", fname);
//             return 1;
//         }
//         else 
//         {
//             // write the data
//             fwrite(record->data, 1, record->len, ofp);

//             // close the file
//             fclose(ofp);
//         }
//     }
//     printf("\n");
//     return 0;
// }

void freebni(BNI *bni)
{
    // free record data
    for(int i = bni->header->num_entries - 1; i > -1; i--)
    {
        BNI_RECORD *record = ((*bni).records) + i;

        free(record->data);
    }

    // free record header
    free(bni->header);
    free(bni->records);
}