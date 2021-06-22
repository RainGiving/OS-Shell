#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#define MAX 5000
using namespace std;
bool flag = false; // flag == false表示输入的ls, flag == true表示输入的ls -l

typedef struct attribute
{
    char mode[15];      // 文件的权限和属性
    int links;          // 链接数
    char user_name[20]; // 用户名
    char group[20];     // 所在用户组
    long size;          // 文件大小
    char mtime[30];     // 最后修改时间
    char filename[20];  // 文件名
    char mon[5];        // 月份
    char hour[5];       // 时
    char min[5];        // 分
    int st_mode;        // 文件类型和权限

} attribute;

char *file_mode(mode_t m, char *str);                      //获取文件模式
int dir_count(char *path);                                 //获取目录数量
int is_dir(struct dirent *dirent);                         //是否是目录或目录下有目录
char *file_user_name(uid_t st_uid, char *str);             //获取用户名
char *file_group(uid_t st_uid, char *str);                 //获取组名
off_t file_size(struct stat buf);                          //获取文件大小
char *file_time(time_t mt, char *str);                     //获取最后修改时间
int file_count(char *path);                                //获取文件数量
void equal(attribute *a, attribute *b);                    //交换
void sort(attribute *info, int index);                     //排序
void show_ls(attribute *info, int index);                  //输出结构体(简略ls)
void show_ls_l(attribute *info, int index);                //输出结构体(详细ls -l)
attribute *create(struct stat buf, struct dirent *dirent); //创建结构体，赋值

int main(int argc, char *argv[])
{
    cout << "************物联网1902耿钰清201916070218***************" << endl;
    cout << "*****************操作系统实验四************************" << endl;
    while (true)
    {
        cout << "请输入 ls 查看目录下简略文件信息 或输入 ls -l 查看详细信息" << endl;
        cout << "输入其他任意内容退出程序" << endl;
        string ls;
        getline(cin, ls);
        if (ls == "ls")
            flag = false;
        else if (ls == "ls -l")
            flag = true;
        else
        {
            cout << "error，程序自动退出" << endl;
            return -1;
        }
        attribute info[MAX];

        char *a = ".";
        char *b = "..";
        char path[10000];
        strcpy(path, "./"); // 只支持当前路径
        int count = file_count(path);

        DIR *dir;
        dir = opendir(path);
        struct dirent *dirent;
        int index = 0; // 结构体下标
        int blocks = 0;
        for (int i = 0; i < count; i++)
        {
            dirent = readdir(dir);
            struct stat buf = {};
            if (stat(dirent->d_name, &buf))
            {
                perror("stat");
                return -1;
            }

            // 跳过特殊情况
            if (strcmp(dirent->d_name, a) == 0 || strcmp(dirent->d_name, b) == 0)
                continue;
            blocks += buf.st_blocks;
            info[index++] = *create(buf, dirent);
        }
        closedir(dir);

        printf("total %d\n", blocks / 2);
        sort(info, index);
        if (flag == true)
            show_ls_l(info, index);
        else
            show_ls(info, index);
        cout << endl;
    }
    return 0;
}

// 输出结构体(详细)
void show_ls_l(attribute *info, int index)
{
    for (int i = 0; i < index; i++)
    {
        printf("%s \033[0m", info[i].mode);
        printf("%d ", info[i].links);
        printf("%s ", info[i].user_name);
        printf("%s ", info[i].group);
        printf("%5ld ", info[i].size);
        printf(" %s ", info[i].mtime);
        if (16893 == info[i].st_mode)
        {
            // 颜色
            printf("\033[34m\033[1m%s\033[0m", info[i].filename);
        }
        else if (33277 == info[i].st_mode)
        {
            printf("\033[32m\033[1m%s\033[0m", info[i].filename);
        }
        else
        {
            printf("%s", info[i].filename);
        }
        if (i < index)
            printf("\n");
    }
}

//输出结构体(简略)
void show_ls(attribute *info, int index)
{
    for (int i = 0; i < index; i++)
    {
        if (16893 == info[i].st_mode)
        {
            // 颜色
            printf("\033[34m\033[1m%s\033[0m", info[i].filename);
        }
        else if (33277 == info[i].st_mode)
        {
            printf("\033[32m\033[1m%s\033[0m", info[i].filename);
        }
        else
        {
            printf("%s  ", info[i].filename);
        }
    }
}

// 获取文件的模式
char *file_mode(mode_t m, char *str)
{
    if (S_ISREG(m))
        str[0] = '-';
    else if (S_ISDIR(m))
        str[0] = 'd';
    else if (S_ISCHR(m))
        str[0] = 'c';
    else if (S_ISBLK(m))
        str[0] = 'b';
    else if (S_ISFIFO(m))
        str[0] = 'q';
    else if (S_ISLNK(m))
        str[0] = 'l';
    else
        str[0] = '?';

    str[1] = '\0';

    strcat(str, S_IRUSR & m ? "r" : "-");
    strcat(str, S_IWUSR & m ? "w" : "-");
    strcat(str, S_IXUSR & m ? "x" : "-");

    strcat(str, S_IRGRP & m ? "r" : "-");
    strcat(str, S_IWGRP & m ? "w" : "-");
    strcat(str, S_IXGRP & m ? "x" : "-");

    strcat(str, S_IROTH & m ? "r" : "-");
    strcat(str, S_IWOTH & m ? "w" : "-");
    strcat(str, S_IXOTH & m ? "x" : "-");

    return str;
}

// 获取目录的数量
int dir_count(char *path)
{
    DIR *dir;
    dir = opendir(path);
    struct dirent *dirent;
    int count = 0;
    while ((dirent = readdir(dir)) != NULL)
    {
        if (dirent->d_type == 4)
            count++;
    }
    closedir(dir);
    return count;
}

// 是否是目录或目录下有目录
int is_dir(struct dirent *dirent)
{
    char *a = dirent->d_name;
    if (dirent->d_type == 8)
        return 1;
    if (dirent->d_type == 4)
    {
        if (dir_count(a) == 0)
            return 2;
        else
            return dir_count(a);
    }
}

// 获取用户名
char *file_user_name(uid_t st_uid, char *str)
{
    struct passwd *user_name;
    user_name = getpwuid(st_uid);
    sprintf(str, "%s", user_name->pw_name);
    return str;
}

// 获取组名
char *file_group(uid_t st_uid, char *str)
{
    struct passwd *user_name;
    user_name = getpwuid(st_uid);
    struct group *grp;
    grp = getgrgid(user_name->pw_gid);
    sprintf(str, "%s", grp->gr_name);
    return str;
}

// 获取文件大小
off_t file_size(struct stat buf)
{
    off_t size = buf.st_size;
    return size;
}

// 获取最后修改时间
char *file_time(time_t mt, char *str)
{
    struct tm *t = localtime(&mt);
    sprintf(str, "%d月 %02d %02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
    return str;
}

// 获取文件的数量
int file_count(char *path)
{
    DIR *dir;
    dir = opendir(path);
    struct dirent *dirent;
    int count = 0;
    while ((dirent = readdir(dir)) != NULL)
    {
        count++;
    }
    closedir(dir);
    return count;
}

// 交换
void equal(attribute *a, attribute *b)
{
    strcpy(a->mode, b->mode);
    a->links = b->links;
    strcpy(a->user_name, b->user_name);
    strcpy(a->group, b->group);
    a->size = b->size;
    strcpy(a->mtime, b->mtime);
    a->st_mode = b->st_mode;
    strcpy(a->filename, b->filename);
}

// 排序
void sort(attribute *info, int index)
{
    attribute *temp = (attribute *)malloc(sizeof(attribute));
    for (int i = index - 1; i > 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (strcmp(info[i].filename, info[j].filename) < 0)
            {
                equal(temp, &info[i]);
                equal(&info[i], &info[j]);
                equal(&info[j], temp);
            }
        }
    }
}

// 创建结构体，赋值
attribute *create(struct stat buf, struct dirent *dirent)
{
    attribute *info = (attribute *)malloc(sizeof(attribute));
    char str[50] = {};
    //puts(file_mode(buf.st_mode,str));
    strcpy(info->mode, file_mode(buf.st_mode, str));
    //puts(info->mode);
    info->links = is_dir(dirent);
    strcpy(info->user_name, file_user_name(buf.st_uid, str));
    strcpy(info->group, file_group(buf.st_uid, str));
    info->size = file_size(buf);
    strcpy(info->mtime, file_time(buf.st_mtime, str));
    info->st_mode = buf.st_mode;
    strcpy(info->filename, dirent->d_name);

    return info;
}