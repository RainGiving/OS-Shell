#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

int main()
{

    struct stat buf;
    int fst = stat("main.cpp", &buf);
    if (fst == -1)
    {
        perror("tips message:");
    }
    printf("buf->st_dev = %ld\n", buf.st_dev);
    printf("buf->st_ino = %ld\n", buf.st_ino);
    printf("buf->st_mode = %d\n", buf.st_mode);
    printf("buf->st_nlink = %ld\n", buf.st_nlink);
    return 0;
}