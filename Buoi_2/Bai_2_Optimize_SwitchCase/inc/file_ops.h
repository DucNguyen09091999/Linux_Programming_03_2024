#ifndef __FILE_OPS_H__
#define __FILE_OPS_H__

#include "common.h"
typedef int32_t status_t;
typedef struct
{
    int fd;
    char *filename;
} FileHandler;

struct OpsType {
    status_t (*pOpenFile)(FileHandler *file, const int mode);
    status_t (*pCloseFile)(FileHandler *file);
    status_t (*pReadFile)(FileHandler *file);
    status_t (*pWriteFile)(FileHandler *file, const char *content);
    off_t (*pGetCurrentOffset)(FileHandler *file);
    status_t (*pRemoveOneLine)(FileHandler *file, int lineNumber);
};


// Forward declaration of the OpsType structure
struct OpsType;

// Function prototypes
status_t OpenFile(FileHandler *file, const int mode);
status_t CloseFile(FileHandler *file);
status_t WriteToFile(FileHandler *file, const char *content);
status_t ReadFromFile(FileHandler *file);
status_t RemoveOneLineInFile(FileHandler *file, int lineNumber);
off_t GetCurrentOffset(FileHandler *file);
extern const struct OpsType File_Operations;

#endif