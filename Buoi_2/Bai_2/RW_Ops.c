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
    if (access(filename, F_OK) == -1) {
        // If file is not exist, create a file
        file->fd = open(filename, O_RDWR | O_CREAT, mode);
        if (file->fd == -1)
        {
            perror("open");
            return E_FAILED;
        }
    }
    else
    {
        // If file is existed, open file
        file->fd = open(filename, O_RDWR, mode);
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
            printf("OK: Tong so dong la %d\n", currentLine);
            return 1; // Dòng tồn tại
        }
    }
    printf("Fail: Tong so dong la %d\n", currentLine);    
    return 0;
}

static void RemoveLine(FileHandler *file, const char *filename, int lineNumber)
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

    // Mở lại tệp gốc
    file->fd = open(filename, O_RDWR, 0666);
    if (file->fd == -1) {
        perror("open");
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

struct OpsType {
    status_t (*pOpenFile)(FileHandler *file, const char *filename, const int mode);
    status_t (*pCloseFile)(FileHandler *file);
    status_t (*pReadFile)(FileHandler *file);
    status_t (*pWriteFile)(FileHandler *file, const char *content);
    status_t (*pLog)( const char *operation, status_t t_status);
    off_t (*pGetCurrentOffset)(FileHandler *file);
    status_t (*pRemoveOneLine)(FileHandler *file, const char *filename, int lineNumber);
};



const struct OpsType File_Operations = {
    .pOpenFile = OpenFile,
    .pCloseFile = CloseFile,
    .pReadFile = ReadFromFile,
    .pWriteFile = WriteToFile,
    .pGetCurrentOffset = GetCurrentOffset,
    .pLog = LogOperation,
    .pRemoveOneLine = RemoveOneLineInFile,
};

void Print_Menu()
{
    printf("============================== Welcome to file operation ===============================\n");
    printf("========================================================================================\n");
    printf("============================== Please Enter your Option  ===============================\n");
    printf("==-------------- Option ------------------||--------------- Feature ------------------==\n");
    printf("========================================================================================\n");
    printf("== 1. Open                                ||  Open the file                           ==\n");
    printf("== 2. Close                               ||  Close the file                          ==\n");
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
    struct OpsType Rw_Ops;
    status_t t_status;
    off_t offset = 0;
    int option;
    char *filename = NULL;
    size_t bufsize = 100;
    size_t filesize;
    int c;
    char *dataFromUser = NULL;
    size_t datasize;

    int lineNumber;

    Rw_Ops.pOpenFile = OpenFile;
    Rw_Ops.pCloseFile = CloseFile;
    Rw_Ops.pReadFile = ReadFromFile;
    Rw_Ops.pWriteFile =  WriteToFile;
    Rw_Ops.pGetCurrentOffset = GetCurrentOffset;
    Rw_Ops.pLog = LogOperation;
    Rw_Ops.pRemoveOneLine = RemoveOneLineInFile;


    Print_Menu();
    while (1)
    {
        printf("Please enter your choice: ");
        scanf("%d", &option);
        switch (option)
        {
            case 1:
                    filename = (char*)malloc(bufsize*sizeof(char));
                    if (filename == NULL)
                    {
                        fprintf(stderr, "Memory allocation error\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Please enter you file name: ");
                // Clear the input buffer to avoid any unexpected behavior because scanf is using before, so the size of string is not 100-bufsize, it is the value which is the integer value previously entered
                /*Using loop to clear input buffer by reading all characteristic from stdin until it reaches newline char or EOF*/
                while ((c = getchar()) != '\n' && c != EOF);
                filesize = getline(&filename, &bufsize, stdin);
                if (filename[filesize - 1] == '\n') {
                    filename[filesize - 1] = '\0';
                }
                t_status = Rw_Ops.pOpenFile(&file, filename, 0667);
                Rw_Ops.pLog("Open", t_status);
                break;
            case 2:
                t_status = Rw_Ops.pCloseFile(&file);
                Rw_Ops.pLog( "Close", t_status);
                free(filename);
                break;
            case 3:
                dataFromUser = (char*)malloc(bufsize*sizeof(char));
                if (dataFromUser == NULL)
                {
                    fprintf(stderr, "Memory allocation error\n");
                    exit(EXIT_FAILURE);
                }
                printf("Please enter you file name: ");
                // Clear the input buffer to avoid any unexpected behavior because scanf is using before, so the size of string is not 100-bufsize, it is the value which is the integer value previously entered
                /*Using loop to clear input buffer by reading all characteristic from stdin until it reaches newline char or EOF*/
                while ((c = getchar()) != '\n' && c != EOF);
                datasize = getline(&dataFromUser, &bufsize, stdin);
                if (dataFromUser[datasize - 1] == '\n') {
                    dataFromUser[datasize - 1] = '\0';
                }
                t_status = Rw_Ops.pWriteFile(&file, dataFromUser);
                Rw_Ops.pLog("Write", t_status);
                free(dataFromUser);
                break;
            case 4:
                t_status = Rw_Ops.pReadFile(&file);
                Rw_Ops.pLog("Read", t_status);
                break;
            case 5:
                offset= Rw_Ops.pGetCurrentOffset(&file);
                printf("Current offset is %ld\n",offset);
                break;
            case 6:
                printf("Please enter the line you want to delete: ");
                while ((c = getchar()) != '\n' && c != EOF);
                scanf("%d",&lineNumber);
                t_status = Rw_Ops.pRemoveOneLine(&file, filename, lineNumber);
                Rw_Ops.pLog("Remove a line in file ", t_status);
                break;
            case 7:
                return 0;
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    }
    return 0;
}