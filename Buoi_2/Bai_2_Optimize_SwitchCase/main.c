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


extern void SelectFile();
extern void FreeFile();
extern void WriteDataToFile();
extern void ReadDataFromFile();
extern void GetCurrentOffsetOfFile();
extern void DeleteLineFromFile();
extern void Print_Menu();

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