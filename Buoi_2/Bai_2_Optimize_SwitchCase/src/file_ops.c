#include "common.h"

// Definition of the OpsType structure
const struct OpsType File_Operations = {
    .pOpenFile = OpenFile,
    .pCloseFile = CloseFile,
    .pReadFile = ReadFromFile,
    .pWriteFile = WriteToFile,
    .pGetCurrentOffset = GetCurrentOffset,
    .pRemoveOneLine = RemoveOneLineInFile,
};


/*
 * @brief                               Open file 
 * @param[in]               file        struct of file
 * @param[in]               mode        Mode of creating file
 * @param[out]              None
 * @return                  status_t    status
 */
status_t OpenFile(FileHandler *file, const int mode)
{
    /*Check if the file is existed or not*/
    if (access(file->filename, F_OK) == -1) {
        /*If file is not exist, create a file*/ 
        file->fd = open(file->filename, O_RDWR | O_CREAT, mode);
        if (file->fd == -1)
        {
            perror("open");
            return E_FAILED;
        }
    }
    else
    {
        /*If file is existed, open file*/ 
        file->fd = open(file->filename, O_RDWR, mode);
        if (file->fd == -1) {
            perror("open");
            return E_FAILED;
        }
    }
    return E_SUCCESS;
}

/*
 * @brief                               close file 
 * @param[in]               file        struct of file
 * @param[out]              None
 * @return                  status_t    status
 */
status_t CloseFile(FileHandler *file)
{
    close(file->fd);
    return E_SUCCESS;
}


/*
 * @brief                               Write data to file
 * @param[in]               file        struct of file
 * @param[in]               content     Content import to file
 * @param[out]              None
 * @return                  status_t    status
 */
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
    /*Write data to file*/
    WriteNum = write(file->fd, content, strlen(content));
    if (WriteNum < 0)
    {
        perror("write");
        return E_FAILED;
    }
    return E_SUCCESS; 
}


/*
 * @brief                               Read data from file 
 * @param[in]               file        struct of file
 * @param[out]              None
 * @return                  status_t    status
 */
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
    /*Read data from file*/
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

/*
 * @brief                                   Check the line is exist or not
 * @param[in]               file            struct of file
 * @param[in]               line_number     line number within a file
 * @param[out]              None
 * @return                  status_t    status
 */
int LineExists(FileHandler *file, int line_number) {
    char ch;
    int currentLine = 0;
    lseek(file->fd, 0, SEEK_SET);
    /* Read each characteristic in file*/
    while ((read(file->fd, &ch, 1)) != 0) {
        /*When reach the EOF, increase curentline*/
        if (ch == '\n') {
            currentLine++;
        }

        /*when matched with expected line, close file and return*/
        if (currentLine == line_number) {
            return 1; /*Line existed*/
        }
    }   
    return 0;
     /*Line is not exist*/
}


/*
 * @brief                                   Remove a line in file
 * @param[in]               file            struct of file
 * @param[in]               line_number     line number within a file
 * @param[out]              None
 * @return                  status_t    status
 */
void RemoveLine(FileHandler *file, int lineNumber)
{
    char buffer[512];
    int currentLine = 0;

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
    if (remove(file->filename) != 0) {
        perror("remove");
        return;
    }

    if (rename("temp.txt", file->filename) != 0) {
        perror("rename");
        return;
    }

}

status_t RemoveOneLineInFile(FileHandler *file, int lineNumber)
{
    // Check whether the line is existed in file or not
    if (!LineExists(file, lineNumber)) {
        // printf("Line %d does not exist.\n", lineNumber);
        return E_FAILED;
    }
    // printf("Line %d exist.\n", lineNumber);
    RemoveLine(file, lineNumber);
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