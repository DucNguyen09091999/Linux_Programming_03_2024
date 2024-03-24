#include <stdio.h>
#include <stdlib.h>
typedef struct
{
    int fd;
}FileHandler;


void OpenFile(FileHandler file, const char *filename, const char *mode)
{
    printf("Open file\n");
}

void CloseFile(FileHandler file)
{
    printf("Close file\n");
}

void WriteToFile(FileHandler file, const char *content)
{
    printf("write to file\n");
}

void ReadFromFile(FileHandler file)
{
    printf("write to file\n");
}

struct OpsType {
    void (*pOpenFile)(FileHandler file, const char *filename, const char *mode);
    void (*pCloseFile)(FileHandler file);
    void (*pReadFile)(FileHandler file);
    void (*pWriteFile)(FileHandler file, const char *content);
};
struct CtrlType {
    const struct OpsType *const pOps;
};

const struct OpsType File_Operations = {
    .pOpenFile = OpenFile,
    .pCloseFile = CloseFile,
    .pReadFile = ReadFromFile,
    .pWriteFile = WriteToFile,
};

int main(int argc, char *argv[])
{
    FileHandler file;
    struct OpsType Rw_Ops;
    Rw_Ops.pOpenFile = OpenFile;
    Rw_Ops.pCloseFile = CloseFile;
    Rw_Ops.pReadFile = ReadFromFile;
    Rw_Ops.pWriteFile =  WriteToFile;

    Rw_Ops.pOpenFile(file,argv[1],"w");
    Rw_Ops.pCloseFile(file);
    Rw_Ops.pReadFile(file);
    Rw_Ops.pWriteFile(file,"wew");

    return 0;
}