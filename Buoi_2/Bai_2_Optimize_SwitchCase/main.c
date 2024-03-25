#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "file_ops.h"
#include "log.h"


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