#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include "file_ops.h"
#include "log.h"

//Declare boolean variable to check whether file is allocated or not
bool isFileAllocated = false;

extern void SelectFile();
extern void FreeFile();
extern void WriteDataToFile();
extern void ReadDataFromFile();
extern void GetCurrentOffsetOfFile();
extern void DeleteLineFromFile();
extern void Print_Menu();

int main(int argc, char *argv[])
{
    (void)argc; // Lời ghi chú để loại bỏ cảnh báo "unused parameter"
    (void)argv; // Lời ghi chú để loại bỏ cảnh báo "unused parameter"

    FileHandler file;
    int option;
    size_t bufsize = 100;


    Print_Menu();
    while (1)
    {
        printf("Please enter your choice: ");
        scanf("%d", &option);
        switch (option)
        {
            case 1:
                if (isFileAllocated)
                {
                    FreeFile(&file);
                }
                SelectFile(&file, bufsize);
                isFileAllocated = true;
                break;
            case 2:
                if (isFileAllocated)
                {
                    FreeFile(&file);
                    isFileAllocated = false;
                }
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
                if (isFileAllocated)
                {
                    FreeFile(&file);
                    isFileAllocated = false;
                }
                return 0;
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    }
    return 0;
}