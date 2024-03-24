#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef int32_t status_t;
#define E_SUCCESS   0
#define E_FAILED    -1

typedef struct
{
    int fd;
} FileHandler;

status_t OpenFile(FileHandler *file, const char *filename, const int mode)
{
    file->fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, mode);
    if (file->fd == -1)
    {
        perror("open");
        return E_FAILED;
    }
    return E_SUCCESS;
}

status_t CloseFile(FileHandler *file)
{
    close(file->fd);
    return E_SUCCESS;
}

status_t WriteToFile(FileHandler *file, const char *content)
{
    ssize_t WriteNum;
    off_t offset;
    /*Seek to end of file to write*/
    offset = lseek(file->fd, 0, SEEK_END);
    if (offset == -1)
    {
        perror("lseek");
        return E_FAILED;
    } 
    WriteNum = write(file->fd, content, strlen(content));
    if (WriteNum < 0)
    {
        perror("write");
        return E_FAILED;
    }
    return E_SUCCESS; 
}

status_t ReadFromFile(FileHandler *file)
{
    ssize_t ReadNum;
    char buffer[512];
    off_t offset;
    /*Seek to head file to read*/
    offset = lseek(file->fd, 0, SEEK_SET);
    if (offset == -1)
    {
        perror("lseek");
        return E_FAILED;
    }
    ReadNum = read(file->fd, buffer, sizeof(buffer));
    if (ReadNum <= 0)
    {
        perror("read");
        return E_FAILED;
    }
    buffer[ReadNum] = '\0';
    printf("Read: %s\n",buffer);
    return E_SUCCESS;
}

status_t LogOperation(const char *operation, status_t t_status)
{
    printf("%s %s\n",operation, (t_status == E_SUCCESS) ? "Successfully" : "Failed");
    return t_status;
}

off_t getCurrentOffset(FileHandler *file)
{
    off_t offset = lseek(file->fd, 0, SEEK_CUR); // Lấy vị trí hiện tại của con trỏ tập tin
    if (offset == -1)
    {
        perror("lseek");
    }
    else
    {
        // printf("Current offset: %ld\n", offset);
    }
    return offset;
}

struct OpsType {
    status_t (*pOpenFile)(FileHandler *file, const char *filename, const int mode);
    status_t (*pCloseFile)(FileHandler *file);
    status_t (*pReadFile)(FileHandler *file);
    status_t (*pWriteFile)(FileHandler *file, const char *content);
    status_t (*pLog)( const char *operation, status_t t_status);
    off_t (*pGetCurrentOffset)(FileHandler *file);
};



const struct OpsType File_Operations = {
    .pOpenFile = OpenFile,
    .pCloseFile = CloseFile,
    .pReadFile = ReadFromFile,
    .pWriteFile = WriteToFile,
    .pGetCurrentOffset = getCurrentOffset,
    .pLog = LogOperation,
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FileHandler file;
    struct OpsType Rw_Ops;
    status_t t_status;
    off_t offset = 0;
    Rw_Ops.pOpenFile = OpenFile;
    Rw_Ops.pCloseFile = CloseFile;
    Rw_Ops.pReadFile = ReadFromFile;
    Rw_Ops.pWriteFile =  WriteToFile;
    Rw_Ops.pGetCurrentOffset = getCurrentOffset;
    Rw_Ops.pLog = LogOperation;


    t_status = Rw_Ops.pOpenFile(&file, argv[1], 0667);
    if (Rw_Ops.pLog("Open", t_status) == E_SUCCESS)
    {
        offset= Rw_Ops.pGetCurrentOffset(&file);
        t_status = Rw_Ops.pWriteFile(&file, "tao la duc day\n");
        if (Rw_Ops.pLog("Write", t_status) == E_SUCCESS)
        {
            offset= Rw_Ops.pGetCurrentOffset(&file);
            t_status = Rw_Ops.pReadFile(&file);
            Rw_Ops.pLog("Read", t_status);

        }
    }
    Rw_Ops.pLog( "Close", Rw_Ops.pCloseFile(&file));

    return 0;
}