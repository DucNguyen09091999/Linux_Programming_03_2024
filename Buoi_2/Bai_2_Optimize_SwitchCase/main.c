#include "common.h"

extern void SelectFile();
extern void FreeFile();
extern void WriteDataToFile();
extern void ReadDataFromFile();
extern void GetCurrentOffsetOfFile();
extern void DeleteLineFromFile();
extern void Print_Menu();

/*Define function pointer*/
typedef void (*MenuFunction)(FileHandler *, size_t);


/*Array to save the features*/
MenuFunction menuFunctions[] = {
    SelectFile,
    FreeFile,
    WriteDataToFile,
    ReadDataFromFile,
    GetCurrentOffsetOfFile,
    DeleteLineFromFile,
    FreeFile // Exit
};

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    FileHandler file;
    int option;
    size_t bufsize = 100;


    Print_Menu();
    while (1)
    {
        printf("Please enter your choice: ");
        scanf("%d", &option);

        if (option < 1 || option > 7) {
            printf("Invalid option\n");
            continue;
        }

        // Call the corresponding features
        menuFunctions[option - 1](&file, bufsize);

        if (option == 7) {
            break; // if option is equal to 7, exit program
        }
    }
    return 0;
}