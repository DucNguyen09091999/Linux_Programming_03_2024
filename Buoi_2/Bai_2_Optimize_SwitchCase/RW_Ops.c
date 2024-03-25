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
    char *filename;
} FileHandler;

// Forward declaration of the OpsType structure
struct OpsType;
struct LogType;
struct OpsType {
    status_t (*pOpenFile)(FileHandler *file, const int mode);
    status_t (*pCloseFile)(FileHandler *file);
    status_t (*pReadFile)(FileHandler *file);
    status_t (*pWriteFile)(FileHandler *file, const char *content);
    off_t (*pGetCurrentOffset)(FileHandler *file);
    status_t (*pRemoveOneLine)(FileHandler *file, const char *filename, int lineNumber);
};

struct LogType {
    status_t (*pLog)( const char *operation, status_t t_status);
};

// Function prototypes
status_t OpenFile(FileHandler *file, const int mode);
status_t CloseFile(FileHandler *file);
status_t WriteToFile(FileHandler *file, const char *content);
status_t ReadFromFile(FileHandler *file);
status_t RemoveOneLineInFile(FileHandler *file, const char *filename, int lineNumber);
status_t LogOperation(const char *operation, status_t t_status);
off_t GetCurrentOffset(FileHandler *file);

const struct LogType Log_Operation = {
    .pLog = LogOperation
};

// Definition of the OpsType structure
const struct OpsType File_Operations = {
    .pOpenFile = OpenFile,
    .pCloseFile = CloseFile,
    .pReadFile = ReadFromFile,
    .pWriteFile = WriteToFile,
    .pGetCurrentOffset = GetCurrentOffset,
    .pRemoveOneLine = RemoveOneLineInFile,
};

status_t OpenFile(FileHandler *file, const int mode)
{
    if (access(file->filename, F_OK) == -1) {
        // If file is not exist, create a file
        file->fd = open(file->filename, O_RDWR | O_CREAT, mode);
        if (file->fd == -1)
        {
            perror("open");
            return E_FAILED;
        }
    }
    else
    {
        // If file is existed, open file
        file->fd = open(file->filename, O_RDWR, mode);
        if (file->fd == -1) {
            perror("open");
            return E_FAILED;
        }
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

// Check the line is exist or not
int LineExists(FileHandler *file, int line_number) {
    char ch;
    int currentLine = 0;
    lseek(file->fd, 0, SEEK_SET);
    // Đọc từng ký tự trong tệp
    while ((read(file->fd, &ch, 1)) != 0) {
        // Nếu gặp ký tự kết thúc dòng, tăng biến đếm dòng
        if (ch == '\n') {
            currentLine++;
        }

        // Nếu chúng ta đến dòng chúng ta quan tâm, đóng tệp và trả về 1
        if (currentLine == line_number) {
            return 1; // Dòng tồn tại
        }
    }   
    return 0;
}

void RemoveLine(FileHandler *file, const char *filename, int lineNumber)
{
    char buffer[512];
    int currentLine = 0;
    size_t bytesRead;
    size_t bytesWritten;

    lseek(file->fd, 0, SEEK_SET);

    int temp_fd = open("temp.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (temp_fd == -1)
    {
        perror("open");
        return;
    }
    // Copy contents with buffer limit of 512 until entire file is copied
    ssize_t total_bytes_copied = 0;
    for (;;) {
        ssize_t bytes_read = read(file->fd, buffer, 1);
        if (bytes_read <= 0) {
            break;
        }

        if (currentLine != lineNumber) {
            ssize_t bytes_written = write(temp_fd, buffer, bytes_read);
            if (bytes_written != bytes_read) {
                printf("bytes_written != bytes_read failed\n");
                printf("bytes_read: %zd\n", bytes_read);
                printf("bytes_written: %zd\n", bytes_written);
                close(file->fd);
                close(temp_fd);
                exit(EXIT_FAILURE);
            }
        }

        if (buffer[0] == '\n') {
            currentLine++;
        }

        total_bytes_copied += bytes_read;
    }
    close(file->fd);
    close(temp_fd);

    // Xóa tệp gốc và đổi tên tệp tạm thời thành tên của tệp gốc
    if (remove(filename) != 0) {
        perror("remove");
        return;
    }

    if (rename("temp.txt", filename) != 0) {
        perror("rename");
        return;
    }

}

status_t RemoveOneLineInFile(FileHandler *file, const char *filename, int lineNumber)
{
    // Check whether the line is existed in file or not
    if (!LineExists(file, lineNumber)) {
        // printf("Line %d does not exist.\n", lineNumber);
        return E_FAILED;
    }
    // printf("Line %d exist.\n", lineNumber);
    RemoveLine(file, filename, lineNumber);
    return E_SUCCESS;    
}

status_t LogOperation(const char *operation, status_t t_status)
{
    printf("%s %s\n",operation, (t_status == E_SUCCESS) ? "Successfully" : "Failed");
    return t_status;
}

off_t GetCurrentOffset(FileHandler *file)
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


void SelectFile(FileHandler *file, size_t bufsize)
{
    int c;
    file->filename = (char*)malloc(bufsize * sizeof(char));
    if (file->filename == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    printf("Please enter you file name: ");
    // Clear the input buffer to avoid any unexpected behavior because scanf is using before, so the size of string is not 100-bufsize, it is the value which is the integer value previously entered
    /*Using loop to clear input buffer by reading all characteristic from stdin until it reaches newline char or EOF*/
    while ((c = getchar()) != '\n' && c != EOF);
    getline(&(file->filename), &bufsize, stdin);
    if (file->filename[strlen(file->filename) - 1] == '\n') {
        file->filename[strlen(file->filename) - 1] = '\0';
    }
}

void FreeFile(FileHandler *file)
{
    if (file->filename != NULL)
    {
        free(file->filename);
        file->filename = NULL;
    }
}

void WriteDataToFile(FileHandler *file, size_t bufsize)
{
    status_t t_status;
    char *dataFromUser = NULL;
    dataFromUser = (char *)malloc(bufsize * sizeof(char));
    if (dataFromUser == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    printf("Please enter data to write to file: ");
    int c;
    // Clear the input buffer
    while ((c = getchar()) != '\n' && c != EOF);
    getline(&dataFromUser, &bufsize, stdin);
    if (dataFromUser[strlen(dataFromUser) - 1] == '\n') {
        dataFromUser[strlen(dataFromUser) - 1] = '\0';
    }
    t_status = File_Operations.pOpenFile(file, 0666);
    Log_Operation.pLog("Open", t_status);
    t_status = File_Operations.pWriteFile(file, dataFromUser);
    Log_Operation.pLog("Write", t_status);
    free(dataFromUser);
    t_status = File_Operations.pCloseFile(file);
    Log_Operation.pLog("Close", t_status);
}

void ReadDataFromFile(FileHandler *file)
{
    status_t t_status;
    t_status = File_Operations.pOpenFile(file, 0666);
    Log_Operation.pLog("Open", t_status);
    t_status = File_Operations.pReadFile(file);
    Log_Operation.pLog("Read", t_status);
    t_status = File_Operations.pCloseFile(file);
    Log_Operation.pLog("Close", t_status);
}

void GetCurrentOffsetOfFile(FileHandler *file)
{
    status_t t_status;
    off_t offset;
    t_status = File_Operations.pOpenFile(file, 0666);
    Log_Operation.pLog("Open", t_status);
    offset = File_Operations.pGetCurrentOffset(file);
    printf("Current offset is %ld\n", offset);
    t_status = File_Operations.pCloseFile(file);
    Log_Operation.pLog("Close", t_status);
}

void DeleteLineFromFile(FileHandler *file)
{
    status_t t_status;
    int lineNumber;
    t_status = File_Operations.pOpenFile(file, 0666);
    Log_Operation.pLog("Open", t_status);
    printf("Please enter the line number you want to delete: ");
    scanf("%d", &lineNumber);
    while (getchar() != '\n'); // Clear input buffer
    t_status = File_Operations.pRemoveOneLine(file, file->filename, lineNumber);
    Log_Operation.pLog("Remove a line in file ", t_status);
    t_status = File_Operations.pOpenFile(file, 0666);
    Log_Operation.pLog("Open", t_status);
}



void Print_Menu()
{
    printf("============================== Welcome to file operation ===============================\n");
    printf("========================================================================================\n");
    printf("============================== Please Enter your Option  ===============================\n");
    printf("==-------------- Option ------------------||--------------- Feature ------------------==\n");
    printf("========================================================================================\n");
    printf("== 1. Select a file                       ||  Select the file you want to work        ==\n");
    printf("== 2. Free selected file                  ||  free file                               ==\n");
    printf("== 3. Write to file                       ||  Write data to file                      ==\n");
    printf("== 4. Read from file                      ||  Read data from file                     ==\n");
    printf("== 5. Get the current offset              ||  Get the current offset                  ==\n");
    printf("== 6. Delte one line in file              ||  Delete assigned line in file            ==\n");
    printf("== 7. Exit program                        ||  Exit program                            ==\n");
    printf("========================================================================================\n");
    printf("========================================================================================\n");

}

int main(int argc, char *argv[])
{

    FileHandler file;
    status_t t_status;
    off_t offset = 0;
    int option;
    size_t bufsize = 100;
    size_t filesize;
    int c;
    char *dataFromUser = NULL;
    size_t datasize;

    int lineNumber;

    Print_Menu();
    while (1)
    {
        printf("Please enter your choice: ");
        scanf("%d", &option);
        switch (option)
        {
            case 1:
                //FreeFile(&file);
                SelectFile(&file, bufsize);
                break;
            case 2:
                FreeFile(&file);
                break;
            case 3:
                WriteDataToFile(&file, bufsize);
                break;
            case 4:
                ReadDataFromFile(&file);
                break;
            case 5:
                GetCurrentOffsetOfFile(&file);
                break;
            case 6:
                DeleteLineFromFile(&file);
                break;
            case 7:
                FreeFile(&file);
                return 0;
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    }
    return 0;
}