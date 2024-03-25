#include "common.h"

extern void SelectFile();
extern void FreeFile();
extern void WriteDataToFile();
extern void ReadDataFromFile();
extern void GetCurrentOffsetOfFile();
extern void DeleteLineFromFile();
extern void Print_Menu();


typedef void (*MenuFunction)(FileHandler *, size_t);

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
        
        if (option < 1 || option > 7) {
            printf("Invalid option\n");
            continue;
        }

        // Gọi hàm tương ứng với option được chọn
        menuFunctions[option - 1](&file, bufsize);

        if (option == 7) {
            break; // Thoát khỏi vòng lặp nếu option là Exit
        }
    }
    return 0;
}