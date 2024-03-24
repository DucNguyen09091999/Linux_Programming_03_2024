#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdint.h>
#include <limits.h> //Using path_max
#include <time.h>
#include <libgen.h>

static bool Checking_Absolutely_Path(const char *path)
{
    char buf[10];
    atoi(strncpy(buf,path + 0, 1));
    if(strcmp(buf,"/")==0)
    {
        /*is absolutely path*/
        return true;
    }
    else
    {
        /*is relatively path*/
        return false;
    }
}

static int8_t Get_Argument_Info(const char *path)
{
    struct stat pathStart;
    if(stat(path,&pathStart) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    if(S_ISDIR(pathStart.st_mode))
    {
        /*Return -1 when path is a folder*/
        return -1;
    }
    else if (S_ISREG(pathStart.st_mode))
    {
        /*Return 0 when path is a file*/
        return 0;
    }
    else
    {
        /*Return -2 when path is a not a folder or file*/
        return -2;
    }
}


static void print_File_Info(char *path)
{
    struct stat file_stats;

    if(stat(path, &file_stats) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    printf("file name: %s\n",basename(path));
    switch (file_stats.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");            break;
        case S_IFCHR:  printf("character device\n");        break;
        case S_IFDIR:  printf("directory\n");               break;
        case S_IFIFO:  printf("FIFO/pipe\n");               break;
        case S_IFLNK:  printf("symlink\n");                 break;
        case S_IFREG:  printf("regular file\n");            break;
        case S_IFSOCK: printf("socket\n");                  break;
        default:       printf("unknown?\n");                break;
    }
    printf("Size %ld\n",file_stats.st_size);
    printf("Last status change:       %s", ctime(&file_stats.st_ctime));
    printf("Last file access:         %s", ctime(&file_stats.st_atime));
    printf("Last file modification:   %s", ctime(&file_stats.st_mtime));
    printf("\n");
}

int main(int argc, char *argv[])
{
    bool bIsAbsolutePath;
    int8_t t_8GetArgInfo;
    char absolutePath[PATH_MAX];
    char *Path;
    if ((argc) < 2 || (argc >= 3))
    {
        printf("Syntax Error\nHint:./read_File <path_to_file or file>\n");
        exit(EXIT_FAILURE);
    }

    t_8GetArgInfo = Get_Argument_Info(argv[1]);
    if ((t_8GetArgInfo == -1) || (t_8GetArgInfo == -2))
    {
        /*Out in case ingress params is a folder or is not a file or folder*/
        printf("This is a folder\nPlease provide file\n");
        exit(EXIT_FAILURE);
    }

    bIsAbsolutePath = Checking_Absolutely_Path(argv[1]);
    if (!bIsAbsolutePath)
    {
        if(realpath(argv[1],absolutePath) == NULL)
        {
            perror("Realpath");
            exit(EXIT_FAILURE);
        }
        else
        {
            Path = absolutePath;
        }

    }
    else
    {
        Path = argv[1];
    }
    print_File_Info(Path);

    return 0;
}

