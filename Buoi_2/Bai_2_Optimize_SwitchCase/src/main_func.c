#include "common.h"

//Declare boolean variable to check whether file is allocated or not
static bool isFileAllocated = false;

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

    if (access(file->filename, F_OK) == -1)
    {
        char choice;
        printf("File is not exist.\nDo you want to create a new file? <y\\n>");
        scanf("%c",&choice);

        if (choice == 'y' || choice == 'Y')
        {
            file->fd = open(file->filename, O_RDWR | O_CREAT, 0666);
            if (file->fd == -1)
            {
                perror("open");
            }
            printf("File created successfully\n");
            isFileAllocated = true;
        }
        else if (choice == 'n' || choice == 'N')
        {
            printf("Rejected create file\n");
        }
        else
        {
            printf("Option is invalid. Only y\\n\n");
        }
    }
}

void FreeFile(FileHandler *file)
{
    if (isFileAllocated)
    {

        free(file->filename);
        file->filename = NULL;
        isFileAllocated = false;
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
{
    const char *menuOptions[] = {
        "Select a file",
        "Free selected file",
        "Write to file",
        "Read from file",
        "Get the current offset",
        "Delete one line in file",
        "Exit program"
    };

    const char *menuFeatures[] = {
        "Select the file you want to work",
        "Free file",
        "Write data to file",
        "Read data from file",
        "Get the current offset",
        "Delete assigned line in file",
        "Exit program"
    };

    printf("============================== Welcome to file operation ===============================\n");
    printf("========================================================================================\n");
    printf("============================== Please Enter your Option  ===============================\n");
    printf("==-------------- Option ------------------||--------------- Feature ------------------==\n");
    printf("========================================================================================\n");

    size_t numOptions = sizeof(menuOptions) / sizeof(menuOptions[0]); // Số lượng tùy chọn

    for (size_t i = 0; i < numOptions; ++i) { // Sử dụng size_t thay cho int
        printf("== %zu. %-36s||  %-40s==\n", i + 1, menuOptions[i], menuFeatures[i]);
    }

    printf("========================================================================================\n");
    printf("========================================================================================\n");
}

}
