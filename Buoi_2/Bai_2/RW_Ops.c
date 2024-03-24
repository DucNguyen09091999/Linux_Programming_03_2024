#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>


typedef int32_t status_t;
#define E_SUCCESS   0
#define E_FAILED    -1
typedef struct
{
    int fd;
}FileHandler;


status_t OpenFile(FileHandler file, const char *filename, const char *mode)
{
    printf("Open file\n");
    return E_SUCCESS;
}

status_t CloseFile(FileHandler file)
{
    printf("Close file\n");
    return E_SUCCESS;
}

status_t WriteToFile(FileHandler file, const char *content)
{
    printf("write to file\n");
    return E_SUCCESS;
}

status_t ReadFromFile(FileHandler file)
{
    printf("write to file\n");
    return E_SUCCESS;
}

struct OpsType {
    status_t (*pOpenFile)(FileHandler file, const char *filename, const char *mode);
    status_t (*pCloseFile)(FileHandler file);
    status_t (*pReadFile)(FileHandler file);
    status_t (*pWriteFile)(FileHandler file, const char *content);
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
    status_t t_status;
    Rw_Ops.pOpenFile = OpenFile;
    Rw_Ops.pCloseFile = CloseFile;
    Rw_Ops.pReadFile = ReadFromFile;
    Rw_Ops.pWriteFile =  WriteToFile;

    t_status = Rw_Ops.pOpenFile(file,argv[1],"w");
    if(t_status == E_SUCCESS)
    {
        printf("Open successfully\n");
    }
    else
    {
        printf("Open failed\n");
    }
    t_status = Rw_Ops.pWriteFile(file,"wew");
    t_status = Rw_Ops.pReadFile(file);
    t_status = Rw_Ops.pCloseFile(file);

    return 0;
}