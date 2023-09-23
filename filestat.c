#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
int main(int argc, char *argv[])
{
    char *dir_path;
    if (argc >= 2)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printf("Usage: %s [path/to/dir]\n", argv[0]);
            printf("Prints count of files of each type in directory (or current working directory if directory is ommited)\n");
            return 0;
        }
        dir_path = argv[1];
    }
    else
    {
        dir_path = getcwd(NULL, 0);
    }
    
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Error while opening directory %s (%d)\n", dir_path, errno);
        return 1;
    }

    int types[] = {0, 0, 0, 0, 0, 0, 0, 0};
    while (1)
    {
        struct dirent *dir_file = readdir(dir);
        if (dir_file == NULL)
            break;
        struct stat file_stat;
        char file_path[512];
        sprintf(file_path, "%s/%s", dir_path, dir_file->d_name);
        int result = lstat(file_path, &file_stat);
        if (result != 0)
        {
            printf("Error for file %s (%d)", file_path, errno);
            return 1;
        }
        switch (file_stat.st_mode & __S_IFMT)
        {
        case __S_IFSOCK:
            types[0]++;
            break;
        case __S_IFLNK:
            types[1]++;
            break;
        case __S_IFREG:
            types[2]++;
            break;
        case __S_IFBLK:
            types[3]++;
            break;
        case __S_IFDIR:
            types[4]++;
            break;
        case __S_IFCHR:
            types[5]++;
            break;
        case __S_IFIFO:
            types[6]++;
            break;
        default:
            types[7]++;
            break;
        }
    }
    closedir(dir);

    printf("Statistics for files in directory %s:\n", dir_path);
    printf("Sockets:           %d\n", types[0]);
    printf("Symbolic links:    %d\n", types[1]);
    printf("Regular files:     %d\n", types[2]);
    printf("Block devices:     %d\n", types[3]);
    printf("Directories:       %d\n", types[4]);
    printf("Character devices: %d\n", types[5]);
    printf("FIFOs:             %d\n", types[6]);
    printf("Unknowns:          %d\n", types[7]);
    return 0;
}