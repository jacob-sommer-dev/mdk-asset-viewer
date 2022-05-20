#ifndef __FILESET_H__
#define __FILESET_H__



// const char* MISC[] = {

//     };

const char* LOAD_3[1] = {"MISC/LOAD_3.LBB"};
const char* LOAD_4[1] = {"MISC/LOAD_4.LBB"};
const char* LOAD_5[1] = {"MISC/LOAD_5.LBB"};
const char* LOAD_6[1] = {"MISC/LOAD_6.LBB"};
const char* LOAD_7[1] = {"MISC/LOAD_7.LBB"};
const char* LOAD_8[1] = {"MISC/LOAD_8.LBB"};

const char* FALL3D_1[3] = {
    "FALL3D/FALL3D.BNI",
    "FALL3D/FALL3D_1.MTI",
    "FALL3D/FALL3D.SNI"
    };

const char* FALL3D_2[3] = {
    "FALL3D/FALL3D.BNI",
    "FALL3D/FALL3D_2.MTI",
    "FALL3D/FALL3D.SNI"
    };

const char* FALL3D_3[3] = {
    "FALL3D/FALL3D.BNI",
    "FALL3D/FALL3D_3.MTI",
    "FALL3D/FALL3D.SNI"
    };

const char* FALL3D_4[3] = {
    "FALL3D/FALL3D.BNI",
    "FALL3D/FALL3D_4.MTI",
    "FALL3D/FALL3D.SNI"
    };

const char* FALL3D_5[3] = {
    "FALL3D/FALL3D.BNI",
    "FALL3D/FALL3D_5.MTI",
    "FALL3D/FALL3D.SNI"
    };

const char* STREAM[2] = {
    "STREAM/STREAM.MTI",
    "STREAM/STREAM.BNI"
    };


const char* FILESETS[12] = {
    "LOAD_3",
    "LOAD_4",
    "LOAD_5",
    "LOAD_6",
    "LOAD_7",
    "LOAD_8",
    "FALL3D_1",
    "FALL3D_2",
    "FALL3D_3",
    "FALL3D_4",
    "FALL3D_5",
    "STREAM"
};

typedef struct fileset_s {
    unsigned int size;
    const char** fileset;
} FileSet;

void findFileSet(const char* name, FileSet& set)
{
    if(strcmp(name, "LOAD_3") == 0)
    {
        set.size = 1;
        set.fileset = LOAD_3;
    }
    else if(strcmp(name, "LOAD_4") == 0)
    {
        set.size = 1;
        set.fileset = LOAD_4;
    }
    else if(strcmp(name, "LOAD_5") == 0)
    {
        set.size = 1;
        set.fileset = LOAD_5;
    }
    else if(strcmp(name, "LOAD_6") == 0)
    {
        set.size = 1;
        set.fileset = LOAD_6;
    }
    else if(strcmp(name, "LOAD_7") == 0)
    {
        set.size = 1;
        set.fileset = LOAD_7;
    }
    else if(strcmp(name, "LOAD_8") == 0)
    {
        set.size = 1;
        set.fileset = LOAD_8;
    }
    else if(strcmp(name, "FALL3D_1") == 0)
    {
        set.size = 3;
        set.fileset = FALL3D_1;
    }
    else if(strcmp(name, "FALL3D_2") == 0)
    {
        set.size = 3;
        set.fileset = FALL3D_2;
    }
    else if(strcmp(name, "FALL3D_3") == 0)
    {
        set.size = 3;
        set.fileset = FALL3D_3;
    }
    else if(strcmp(name, "FALL3D_4") == 0)
    {
        set.size = 3;
        set.fileset = FALL3D_4;
    }
    else if(strcmp(name, "FALL3D_5") == 0)
    {
        set.size = 3;
        set.fileset = FALL3D_5;
    }
    else if(strcmp(name, "STREAM") == 0)
    {
        set.size = 2;
        set.fileset = STREAM;
    }
}


#endif //__FILESET_H__