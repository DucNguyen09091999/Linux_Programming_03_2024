#include "file_ops.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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