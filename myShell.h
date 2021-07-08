//-----------------------------------------------------------头文件-------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <bits/stdc++.h>
using namespace std;
#define PATH_SIZE 4094
//--------------------------------------------------------------------------------------------------------------------

//---------------------------------------------声明函数和全局变量-------------------------------------------------------
void deal_cmd(string[]); //识别命令并跳转至各命令对应的函数
void history();          // 查看历史命令记录
void do_last(string);
void writeHistory(string[]); //每次执行命令后将命令保存至history.txt文件
void historyCheck();         //每次开始输入命令前检查history.txt里是否有之前的历史命令，更新history_count
void cp(string[]);           //复制文件1到文件2
void pwd();                  //输出当前工作目录的绝对路径
// void ls(string[]);
void cat(string[]); //显示文件内容
void cal();         //显示日历
void date();        //显示当前日期时间
void mv(string[]);  //将文件移动到指定目录下
char *getFileName(char *fileName);
void rm(string);         //删除文件
void rmdir(string);      //删除目录
void whoami();           //显示当前用户名
void ps(string cmd[]);   //观察系统所有进程数据
void file(string cmd[]); //检查指定文件的属性
void ls(string cmd[]);   //打印当前目录列表
void top(string cmd[]);  //实时显示进程的各种参数
void mkdir(string);      //新建目录
void help();             //打印各个命令的用法及作用
//----------------------------------------------------------------------------------------
//---------------------------------------------cal使用的函数声明----------------------------
int is_leap_year(int year);
int days_of_year(int year);
int days_of_year_month(int year, int month);
int week_of_year_month_day(int year, int month, int day);
void print_cal_year_month(int year, int month);
int check_valid_year(char *year);
int check_valid_month(char *month);
//-------------------------------------------------------------------------

const int maxFileSize = 10000; //最大注册数目
const int maxDirDepth = 256;   // 最大目录深度
int scount = 0;                //当前已注册数目
int history_count = 0;         //历史记录条数

//----------------------------------------------------------------------------------
//-------------------------------------------------实现登录注册功能------------------------------------------------------
//用户类
class User
{
private:
    string username; //账号
    string password; //密码
public:
    User(){};
    void Signup(); //注册
    void Login();  //登录
    void save();   //保存
    void read();   //读取
} us;
User user[maxFileSize];
//保存
void User::save()
{
    ofstream os;                   //创建流
    os.open("user.txt", ios::out); //打开文件，文件和流建立关联

    for (int i = 0; i < scount; i++)
    {
        os << user[i].username << endl; //向文件中写入(程序输出)一行-用户名
        os << user[i].password << endl; //向文件中写入(程序输出)一行-密码
    }
    os.close();
}

//读取
void User::read()
{
    ifstream is;                  //创建流
    is.open("user.txt", ios::in); //打开文件，文件和流建立关联

    scount = 0;

    for (int i = 0; !is.eof(); i++)
    {
        is >> user[i].username; //从文件中读出一行-用户名
        is >> user[i].password; //从文件中读出一行-密码
        scount++;               //账号数量
    }
    is.close();
}

//注册
void User::Signup()
{
    us.read();  //读取已储存用户数据
    string usr; //账号
    while (true)
    {
        bool flag = false; //是否注册了不重复的账号
        printf("请输入你的账号：");
        cin >> usr;
        //判断新输入的用户信息是否已存在（如果已存在则重新输入）
        for (int i = 0; i < scount; i++)
        {
            if (usr == user[i].username)
            {
                printf("该账号已被注册！请选用其他账号\n");
                break;
            }
            if (i == scount - 1) //未发现有重复账号
                flag = true;
        }
        if (flag)
            break; //注册了不重复的账号，可以继续注册
    }
    user[scount].username = usr;
    cout << "请输入你的密码：";
    cin >> user[scount].password;
    scount++;  //注册成功账号数+1
    us.save(); // 保存用户信息
    cout << "注册成功！" << endl;
}

//登录
void User::Login()
{
    us.read();       //读取已储存用户数据
    string usr;      //账号
    string pwd;      //密码
    int index = 0;   //查找到的账号索引
    int maxTime = 5; //最多重新输入密码五次

    while (true)
    {
        bool flag = false; //是否找到记录中的账号
        printf("请输入你的账号：");
        cin >> usr;
        //判断新输入的用户信息是否已存在（如果不存在则重新输入）
        for (int i = 0; i < scount; i++)
        {
            if (usr == user[i].username)
            {
                index = i;
                flag = true;
                break;
            }
            if (i == scount - 1)
                cout << "该账号尚未注册！请重新输入账号" << endl;
        }
        if (flag)
            break; //查询到账号，可以继续登录
    }
    while (maxTime--)
    {
        cout << "请输入你的密码：";
        cin >> pwd;
        if (pwd == user[index].password)
        {
            cout << "登录成功！" << endl;
            break;
        }
        else
            cout << "密码不匹配，请重试！" << endl;
    }
    if (maxTime == -1)
    {
        cout << "密码输入错误次数过多，已自动退出程序" << endl;
        exit(0);
    }
}

//-----------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------实现识别处理命令cmd，并跳转到各命令函数gyq---------------------------------
void deal_cmd(string cmd[])
{
    if (cmd[0] == "history")
    {
        history();
        writeHistory(cmd);
    }
    else if (cmd[0] == "cp")
    {
        cp(cmd);
        writeHistory(cmd);
    }
    else if (cmd[0] == "pwd")
    {
        pwd();
        writeHistory(cmd);
    }
    else if (cmd[0] == "exit")
    {
        cout << "Exit Successfully" << endl;
        writeHistory(cmd);
        exit(0); //退出程序
    }
    else if (cmd[0] == "cat")
    {
        cat(cmd);
        writeHistory(cmd);
    }
    else if (cmd[0] == "cal")
    {
        cal();
        writeHistory(cmd);
    }
    else if (cmd[0] == "date")
    {
        date();
        writeHistory(cmd);
    }
    else if (cmd[0] == "mv")
    {
        mv(cmd);
        writeHistory(cmd);
    }
    else if (cmd[0][0] == '!') //这里先判断开头为!再根据
    {
        do_last(cmd[0]);
    }
    else if (cmd[0] == "ps")
    {
        ps(cmd);
        writeHistory(cmd);
    }
    else if (cmd[0] == "rm")
    {
        rm(cmd[1]);
        writeHistory(cmd);
    }
    else if (cmd[0] == "rmdir")
    {
        rmdir(cmd[1]);
        writeHistory(cmd);
    }
    else if (cmd[0] == "whoami")
    {
        whoami();
        writeHistory(cmd);
    }
    else if (cmd[0] == "file")
    {
        file(cmd);
        writeHistory(cmd);
    }
    else if (cmd[0] == "ls")
    {
        ls(cmd);
        writeHistory(cmd);
    }
    else if (cmd[0] == "top")
    {
        if (cmd[1] == "&") //需要背景执行
        {
            //fpid表示fork()函数的返回值
            pid_t fpid = fork();
            if (fpid < 0) //为负值时，表示fork出现错误
                cout << "ERROR: Failed to fork";
            else if (fpid == 0) //在子进程中fork()返回0
            {
                cout << "当前处于子进程中" << endl;
                //这里设定更新1次后返回
                system("top -n 1"); //希望在新的子进程中运行
            }
            else if (fpid == 1) //在父进程中fork()返回1
                return;
        }
        else
            system("top");
        writeHistory(cmd);
    }
    else if (cmd[0] == "mkdir")
    {
        mkdir(cmd[1]);
        writeHistory(cmd);
    }
    else if (cmd[0] == "help")
    {
        help();
        writeHistory(cmd);
    }
    else
    {
        cout << "Invalid command";
    }
}

//------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------实现查询历史记录功能gyq--------------------------------------------------
void history()
{
    if (history_count == 0) //无历史记录，则提示并返回
    {
        cout << "No history" << endl;
        return;
    }
    ifstream is;                     //创建流
    is.open("history.txt", ios::in); //打开文件，文件和流建立关联
    string his[history_count];
    for (int i = 0; i < history_count; ++i) //一行一行读取历史记录
        getline(is, his[i]);
    if (history_count <= 10) //如果记录小于等于10条，则全部打印
        for (int i = 0; i < history_count; ++i)
            cout << i << "        " << his[i] << endl;
    else // 如果记录大于10条，则打印最近10条
        for (int i = history_count - 10; i < history_count; ++i)
            cout << i << "        " << his[i] << endl;
    is.close();
}

void historyCheck() //检查记录中的历史记录条数，更新history_count
{
    ifstream is;
    is.open("history.txt", ios::in);
    string line;
    while (getline(is, line))
        history_count++;
    is.close();
}

void writeHistory(string cmd[])
{
    FILE *fp;
    if ((fp = fopen("history.txt", "a+")) < 0) //判断是否成功打开记录文件
        perror("Error:History file not open");
    ofstream os;                      //创建流
    os.open("history.txt", ios::app); //打开文件，文件和流建立关联，app是追加方式
    os << cmd[0] << " " << cmd[1] << " " << cmd[2] << endl;
    history_count++;
    os.close();
}
//-------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------实现!!执行最近执行过的命令gyq-----------------------------------------------
void do_last(string cmd)
{
    //读取命令中指定的数字
    string s_num;
    int num;
    if (cmd[1] == '!')
        num = 1; //输入的命令是!!
    else         //输入的命令是!N
    {
        for (int i = 1; i < cmd.size(); i++)
            s_num += cmd[i];
        num = stoi(s_num);
    }
    int his_num = history_count - num;
    if (history_count == 0) //无历史记录，则提示并返回
    {
        cout << "No history" << endl;
        return;
    }
    ifstream is;                     //创建流
    is.open("history.txt", ios::in); //打开文件，文件和流建立关联
    string his[history_count];
    for (int i = 0; i < history_count; ++i) //一行一行读取历史记录
        getline(is, his[i]);
    is.close();
    //因为我们得到的his都是包含空格的一个string，要把它拆开以方便传给deal_cmd
    stringstream ss(his[his_num]);
    string temp;
    string his_cmd[3];
    int i = 0;
    while (ss >> temp)
        his_cmd[i++] = temp;
    deal_cmd(his_cmd);
    return;
}
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------输出当前工作目录的绝对路径gyq-------------------------------------------

//根据文件名(.)获取文件的inode_number
ino_t get_inode_number(char *filename)
{
    struct stat st;
    if (0 != stat(filename, &st))
    {
        perror("stat");
        exit(-1);
    }
    return st.st_ino;
}

//根据inode_number在当前目录查找对应的文件名
char *find_name_byino(ino_t inode)
{
    char *str;
    DIR *dirp;
    struct dirent *dirt;
    if ((dirp = opendir(".")) == NULL)
    {
        perror(".");
        exit(-1);
    }
    while ((dirt = readdir(dirp)) != NULL)
    {
        if (dirt->d_ino == inode)
        {
            str = (char *)malloc(strlen(dirt->d_name) * sizeof(char));
            strcpy(str, dirt->d_name);
            return str;
        }
    }
    perror(".");
    exit(-1);
}

void pwd()
{
    char *dir_stack[maxDirDepth]; //记录目录名的栈
    int current_depth = 0;        //当前目录深度
    while (true)
    {
        char *current = (char *)".";
        char *parent = (char *)"..";
        ino_t current_ino = get_inode_number(current); //获取当前目录的inode_number
        ino_t parent_ino = get_inode_number(parent);   //获取父目录的inode_number

        if (current_ino == parent_ino) //如果这两个inode_number相等，说明到达了根目录
            break;

        //未到达根目录,切换至父目录重复过程，并记录当前文件名
        chdir("..");
        dir_stack[current_depth++] = find_name_byino(current_ino);
        if (current_depth >= maxDirDepth) //路径过深
        {
            cout << "Error: Directory tree is too deep." << endl;
            return;
        }
    }

    //得到完整路径名
    string current_path;
    for (int i = current_depth - 1; i >= 0; i--)
    {
        cout << "/" << dir_stack[i];
        current_path = current_path + "/" + dir_stack[i];
    }
    const char *current_path_str = current_path.c_str();
    chdir(current_path_str); // 最后再把工作目录切换回当前目录，便于下次调用pwd
}

//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------复制文件功能-lwd----------------------------------------------------------------------
void cp(string cmd[])
{
    FILE *fr, *fw;
    char ch;
    if (cmd[1] == " " || cmd[2] == " ")
    {
        cout << "Error：请输入完整文件名";
    }
    const char *cmd1 = cmd[1].c_str();
    const char *cmd2 = cmd[2].c_str();
    fr = fopen(cmd1, "r");
    if (fr == NULL)
    {
        printf("Error：打不开文件");
        return;
    }
    fw = fopen(cmd2, "w");
    if (fw == NULL)
    {
        printf("Error：打不开文件");
        return;
    }
    ch = fgetc(fr);

    while (!feof(fr))
    {
        fputc(ch, fw);
        ch = fgetc(fr);
    }
    fclose(fr);
    fclose(fw);
}

//-----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------cat显示文件内容-qbj---------------------------------------------------------------

void cat(string cmd[])
{
    const char *cmd1 = cmd[1].c_str();
    FILE *fa = fopen(cmd1, "r");
    if (fa == NULL)
    {
        cout << "Error:No such file." << endl;
        return;
    }
    char *buf = (char *)malloc(10000);
    while (fgets(buf, 10000, fa) != NULL)
    {
        printf("%s", buf);
        memset(buf, 0, 10000);
    }
    free(buf);
}

//-----------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------cal显示日历-qbj----------------------------------------------------------------------------
void cal()
{
    time_t now;         //实例化time_t结构
    struct tm *timenow; //实例化tm结构指针
    time(&now);
    //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
    timenow = localtime(&now);
    int year = timenow->tm_year + 1900;
    int month = timenow->tm_mon + 1;
    print_cal_year_month(year, month);
    cout << endl;
}

// judge a year is leap year
int is_leap_year(int year)
{
    if (year < 0)
        return -1;
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
        return 1;
    return 0;
}

// how many days of a year
int days_of_year(int year)
{
    if (year < 0)
        return -1;
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
        return 366;
    return 365;
}

// how many days of a certain month, year
int days_of_year_month(int year, int month)
{
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        return 31;
    else if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) // 闰年2月29天
        return 29;
    return 28;
}

/*
week of day-month-year, from week 1-7

基姆拉尔森计算公式
W = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7, 其中y是年，m是月，d是日，
有两点要注意：当m是一月或2月时，算作上一年的13月或14月，例如：2013 - 1 - 1应记作2012 - 13 - 1
此公式算出来的星期从0开始
*/
int week_of_year_month_day(int y, int m, int d)
{

    if (m == 1 || m == 2)
    {
        m += 12;
        y--;
    }
    int week = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
    return week + 1;
}

void print_cal_year_month(int year, int month)
{
    time_t now;         //实例化time_t结构
    struct tm *timenow; //实例化tm结构指针
    time(&now);
    //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
    timenow = localtime(&now);
    int now_year = timenow->tm_year + 1900;
    int now_month = timenow->tm_mon + 1;
    int now_day = timenow->tm_mday;

    // 居中显示 月份 和 年
    char monthen[][100] = {
        "",
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December",
    };

    char s[100];
    sprintf(s, "%s %d", monthen[month], year);
    int blackAdd = (7 * 3 + 2 - (int)strlen(s)) / 2;
    while (blackAdd--)
        printf(" ");
    printf("%s\n", s);

    // 显示这个月的日历
    int i, j;
    char weeks[][3] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
    for (j = 0; j <= 6; j++)
    {
        if (j == 6)
            printf("%s\n", weeks[j]);
        else
            printf("%s ", weeks[j]);
    }

    int days = days_of_year_month(year, month);        //
    int week = week_of_year_month_day(year, month, 1); //

    int blankNum = 0;
    if (week == 7)
        blankNum = 0;
    else
    {
        blankNum = week;
    }

    for (i = 0; i < blankNum; i++)
        printf("   "); // 3个空格

    for (i = 1; i <= days; i++)
    {
        if (i == days)
        {
            if (year == now_year && month == now_month && now_day == i)
                printf("\033[47;30m%2d\n\033[0m", i);
            else
                printf("%2d\n", i);
            break;
        }
        blankNum++;
        if (blankNum < 7)
            if (year == now_year && month == now_month && now_day == i)
                printf("\033[47;30m%2d \033[0m", i);
            else
                printf("%2d ", i);
        else
        {
            if (year == now_year && month == now_month && now_day == i)
                printf("\033[47;30m%2d\n\033[0m", i);
            else
                printf("%2d\n", i);
            blankNum = 0;
        }
    }
}

// check a string year is "1" to "9999"
int check_valid_year(char *year)
{
    int i;
    int len = strlen(year);
    for (i = 0; i < len; i++)
    {
        if (!(year[i] >= '0' && year[i] <= '9'))
            return -1;
    }
    int y = atoi(year);
    if (y >= 1 && y <= 9999)
        return y;
    return -2;
}

// check a string month is "1" to "12"
int check_valid_month(char *month)
{
    int i;
    int len = strlen(month);
    int y = atoi(month);
    if (y >= 1 && y <= 12)
        return y;

    if (len < 3)
        return -1;

    char s[4];
    for (i = 0; i < 3; i++)
    {
        if (isalpha(month[i]))
        {
            if (i == 0)
                s[i] = toupper(month[i]);
            else
                s[i] = tolower(month[i]);
        }
        else
        {
            return -2;
        }
    }
    s[3] = '\0';
    char monthen[][100] = {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec",
    };
    for (i = 0; i < 12; i++)
    {
        if ((int)strcmp(s, monthen[i]) == 0)
            return i + 1;
    }
    return -2;
}

//--------------------------------------------------------------------------------------------------------------------------

//------------------------------------------date显示当前日期qbj-----------------------------------------------------------------
void date()
{
    time_t now;
    struct tm *t;
    const char *wd[7] = {
        "星期日",
        "星期一",
        "星期二",
        "星期三",
        "星期四",
        "星期五",
        "星期六",
    };

    time(&now);
    t = localtime(&now);
    printf("%d年  %d月  %d日 %s ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, wd[t->tm_wday]);
    printf("%d:%d:%d UTC", t->tm_hour, t->tm_min, t->tm_sec);
}
//------------------------------------------------------------------------------------------------------------

//-----------------------------------mv移动文件qbj-----------------------------------------------------------
// check if it is the same inode on the same device
#define SAME_INODE(a, b) ((a).st_ino == (b).st_ino && (a).st_dev == (b).st_dev)

// ALL CHECKS OMMITTED!

void mv(string cmd[])
{
    const char *cmd1 = cmd[1].c_str();
    const char *cmd2 = cmd[2].c_str();
    struct stat statbuf_src, statbuf_dest;
    char *src, *dest, *new_src, *new_dest;
    char *current_directory;

    // work on copy
    src = (char *)malloc(strlen(cmd1) + 1);
    dest = (char *)malloc(strlen(cmd2) + 1);
    strcpy(src, cmd1);
    strcpy(dest, cmd2);

    stat(src, &statbuf_src);
    stat(dest, &statbuf_dest);

    // there are many more, of course
    printf("\"%s\" is a ", src);
    if (S_ISREG(statbuf_src.st_mode))
    {
        puts("a regular file");
    }
    if (S_ISDIR(statbuf_src.st_mode))
    {
        puts("a directory");
    }

    printf("\"%s\" is a ", dest);
    if (S_ISREG(statbuf_dest.st_mode))
    {
        puts("a regular file");
    }
    if (S_ISDIR(statbuf_dest.st_mode))
    {
        puts("a directory");
    }

    if (SAME_INODE(statbuf_dest, statbuf_src))
    {
        printf("%s and %s are the identical\n", src, dest);
    }
    // if that is not set you have to do it by hand:
    // climb up the tree, concatenating names until the inodes are the same
    current_directory = getenv("PWD");
    printf("current directory is \"%s\"\n", current_directory);

    // I'm pretty sure it can be done in a much more elegant way
    new_src = (char *)malloc(strlen(src) + 1 + strlen(current_directory) + 1);
    strcpy(new_src, current_directory);
    strcat(new_src, "/");
    strcat(new_src, src);
    //printf("new_src = %s\n", new_src);

    new_dest = (char *)malloc(strlen(dest) + 1 + strlen(current_directory) + 1 + strlen(src) + 1);
    strcpy(new_dest, current_directory);
    strcat(new_dest, "/");
    strcat(new_dest, dest);
    strcat(new_dest, "/");
    strcat(new_dest, src);
    printf("new_dest = %s\n", new_dest);

    if (rename(new_src, new_dest) != 0)
    {
        fprintf(stderr, "rename failed with error %s\n", strerror(errno));
    }

    free(new_src);
    free(new_dest);
    free(src);
    free(dest);

    return;
}
//------------------------------------------------------------------------------------------------------------------

//--------------------------------------------PS相关声明-------------------------------

#define MAX_LEN 20
struct ps_info *trav_dir(char dir[]);
int read_info(char d_name[], struct ps_info *p1);
void print_ps(struct ps_info *head);
int is_num(char *);
void uid_to_name(uid_t uid, struct ps_info *p1);
void ps(string cmd[]);
void ps_ps(struct ps_info *head);
void ps_aux(struct ps_info *head);
void ps_axjf(struct ps_info *head);

typedef struct ps_info
{
    char pname[MAX_LEN];
    char user[MAX_LEN];
    int pid;
    int ppid;
    char state;
    struct ps_info *next;
} mps;

//-------------------------------------PS进程相关功能-----------------------------------------------------------------

void ps(string cmd[])
{
    mps *head;
    head = trav_dir((char *)"/proc/");
    if (head == NULL)
        printf("traverse dir error\n");
    if (cmd[1] == " ")
        ps_ps(head); //如果cmd[1] = ''调用，ps_ps()方法
    else if (cmd[1] == "aux")
        ps_aux(head); //如果cmd[1] ='aux'，调用ps_aux()方法
    else if (cmd[1] == "axjf")
        ps_axjf(head); //如果cmd[1] = 'axjf'，调用  ps_axjf()方法，还未实现
    else
        cout << "Error：error: unknown user-defined format specifier " << cmd[1];
}

void ps_ps(struct ps_info *head)
{
    mps *link;

    print_ps(head);

    while (head != NULL)
    {
        link = head;
        head = head->next;
    }
    return;
}
void ps_aux(struct ps_info *head)
{
    int length;
    mps *list1 = NULL;
    list1 = head;
    mps *arr[1001];

    int i = 0, j = 0;
    mps *temp = NULL;

    for (; list1 != NULL; list1 = list1->next, i++)
    {
        arr[i] = list1;
    }
    length = i;
    for (i = 0; i < length - 1; i++)
    {
        for (j = 0; j < length - 1 - i; j++)
        {
            if ((arr[j]->ppid) > (arr[j + 1]->ppid))
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    printf("USER\t\t\t\tPID\t\tPPID\t\tSTATE\t\tPNAME\n");
    for (i = 0; i < length; i++)
    {
        printf("%-16s\t\t%d\t\t%d\t\t%c\t\t%s\n", arr[i]->user, arr[i]->pid, arr[i]->ppid, arr[i]->state, arr[i]->pname);
    }

    return;
}
void ps_axjf(struct ps_info *head)
{
    system("ps axjf");
    return;
}
mps *trav_dir(char dir[])
{
    DIR *dir_ptr;
    mps *head, *p1, *p2;
    struct dirent *direntp;
    struct stat infobuf;

    if ((dir_ptr = opendir(dir)) == NULL)
        fprintf(stderr, "dir error %s\n", dir);
    else
    {
        head = p1 = p2 = (struct ps_info *)malloc(sizeof(struct ps_info)); //建立链表
        while ((direntp = readdir(dir_ptr)) != NULL)                       //遍历/proc目录所有进程目录
        {
            if ((is_num(direntp->d_name)) == 0) //判断目录名字是否为纯数字
            {
                if (p1 == NULL)
                {
                    printf("malloc error!\n");
                    exit(0);
                }

                if (read_info(direntp->d_name, p1) != 0) //获取进程信息
                {
                    printf("read_info error\n");
                    exit(0);
                }
                p2->next = p1; //插入新节点
                p2 = p1;
                p1 = (struct ps_info *)malloc(sizeof(struct ps_info));
            }
        }
    }
    p2->next = NULL;
    return head;
}

int read_info(char d_name[], struct ps_info *p1)
{
    FILE *fd;
    char dir[20];
    struct stat infobuf;

    sprintf(dir, "%s/%s", "/proc/", d_name);
    chdir("/proc");                   //切换至/proc目录，不然stat返回-1
    if (stat(d_name, &infobuf) == -1) //get process USER
        fprintf(stderr, "stat error %s\n", d_name);
    else
        //p1->user=uid_to_name(infobuf.st_uid);
        uid_to_name(infobuf.st_uid, p1);

    chdir(dir); //切换到/proc/pid目录
    if ((fd = fopen("stat", "r")) < 0)
    {
        printf("open the file is error!\n");
        exit(0);
    }
    while (4 == fscanf(fd, "%d %s %c %d\n", &(p1->pid), p1->pname, &(p1->state), &(p1->ppid))) //读文件的前四个字段内容，并存放在相关的链表成员中
    {
        break;
    }
    fclose(fd);
    return 0;
}

void uid_to_name(uid_t uid, struct ps_info *p1) //由进程uid得到进程的所有者user
{
    struct passwd *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr, "%d", uid);
        strcpy(p1->user, numstr);
    }
    else
        strcpy(p1->user, pw_ptr->pw_name);
}

int is_num(char p_name[])
{
    int i, len;
    len = strlen(p_name);
    if (len == 0)
        return -1;
    for (i = 0; i < len; i++)
        if (p_name[i] < '0' || p_name[i] > '9')
            return -1;
    return 0;
}

void print_ps(struct ps_info *head)
{
    mps *list;
    printf("USER\t\t\t\tPID\t\tPPID\t\tSTATE\t\tPNAME\n");
    for (list = head; list != NULL; list = list->next)
    {
        printf("%-16s\t\t%d\t\t%d\t\t%c\t\t%s\n", list->user, list->pid, list->ppid, list->state, list->pname);
    }
}

//--------------------------------------------------------------------------------------------------------------------

//----------------------------------------rm与rmdir-------------------------------------------------------------------
void rmdir(string Path_name)
{
    DIR *dirp;
    const char *path = Path_name.c_str();
    dirp = opendir(path);
    if (NULL == dirp)
    {
        perror(path);
        return;
    }

    struct dirent *entry;
    int ret;
    while (1)
    {
        entry = readdir(dirp);
        if (NULL == entry)
        {
            break;
        }
        // skip . & ..
        if (0 == strcmp(".", entry->d_name) || 0 == strcmp("..", entry->d_name))
        {
            continue;
        }

        char buf[PATH_SIZE];
        snprintf(buf, PATH_SIZE, "%s/%s", path, entry->d_name);
        ret = remove(buf);
        if (-1 == ret)
        {
            if (ENOTEMPTY == errno)
            {
                string buff(buf);
                rmdir(buf);
                continue;
            }
            perror(buf);
            return;
        }
        //fprintf(stdout, "rm file: %s\n", buf);
    }

    closedir(dirp);

    ret = rmdir(path);
    if (-1 == ret)
    {
        perror(path);
        return;
    }
    //fprintf(stdout, "rm dir: %s\n", path);
}

void rm(string Path_name)
{
    const char *path = Path_name.c_str();
    if (remove(path) == 0)
    {
        printf("Deleted successfully");
    }
    else
    {
        printf("Unable to delete the file");
    }
}

//---------------------------------------------------------------------------------------------------------------------

//-----------------------------------------whoami----------------------------------------------------------------------
void whoami()
{
    uid_t id;
    struct passwd *pa;
    id = geteuid();
    pa = getpwuid(id);
    printf("%d\n", id);
    printf("%s", pa->pw_name);
}
//---------------------------------------------------------------------------------------------------------------------

//----------------------------------------file-------------------------------------------------------------------------
void file(string cmd[])
{
    string cmd012 = cmd[0] + " " + cmd[1] + " " + cmd[2];
    const char *cmdd = cmd012.c_str();
    system(cmdd);
    return;
}

//--------------------------------------ls------------------------------------------------------------------------------

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
const int max_files = 4096;
void ls(string cmd[])
{
    bool flag;
    if (cmd[1] == " ")
        flag = false;
    else if (cmd[1] == "-l")
        flag = true;
    else
    {
        cout << "Error: Unknown command";
        return;
    }
    attribute info[max_files];
    char *a = (char *)".";
    char *b = (char *)"..";
    char path[1000];
    strcpy(path, "./"); //只支持当前路径
    int count = file_count(path);

    DIR *dir = opendir(path);
    struct dirent *dirent;
    int index = 0; //结构体下标
    int blocks = 0;
    for (int i = 0; i < count; i++)
    {
        dirent = readdir(dir);
        struct stat buf = {};
        if (stat(dirent->d_name, &buf))
        {
            perror("stat");
            return;
        }
        if (strcmp(dirent->d_name, a) == 0 || strcmp(dirent->d_name, b) == 0) //特殊情况跳过
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
    return;
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

//---------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------mkdir---------------------------------------------------------------
void mkdir(string dir_name)
{
    const char *dir = dir_name.c_str();
    // 判断是否是当前目录或/目录
    if (strcmp(dir, ".") == 0)
    {
        cout << "Error：不能创建当前目录";
        return;
    }
    //判断该目录是否存在
    if (access(dir, F_OK) == 0)
    {
        cout << "Error：当前目录已存在";
        return;
    }
    if (mkdir(dir, 0775) != 0) //创建该目录
        perror("mkdir");
    return;
}

//---------------------------------------------------------------------------------------------------------------------
void help()
{
    cout << "(1)    ps           观察系统所有的进程数据                   " << endl;
    cout << "(2)    ps axjf      连同部分进程树状态显示              " << endl;
    cout << "(3)    ps aux       依照PID的顺序来排序显示      " << endl;
    cout << "(4)    ls -l       打印当前目录下的文件及目录的详细信息 " << endl;
    cout << "(5)    ls           打印当前目录下的文件及目录的简略信息     " << endl;
    cout << "(6)    top          实时显示进程的各种参数                 " << endl;
    cout << "(7)    cal          显示日历                              " << endl;
    cout << "(8)    Whoami       显示当前用户名                     " << endl;
    cout << "(9)    date         显示当前日期时间                      " << endl;
    cout << "(10)   pwd          输出当前工作目录的绝对路径              " << endl;
    cout << "(11)   mv           将文件移动到指定目录下                   " << endl;
    cout << "(12)   cp           复制文档                                 " << endl;
    cout << "(13)   file         输出指定文件的属性                     " << endl;
    cout << "(14)   cat          输出文件内容                           " << endl;
    cout << "(15)   rm           删除文档                                 " << endl;
    cout << "(16)   history      输出近十条历史记录                   " << endl;
    cout << "(17)   !!           执行上一条执行的命令                        " << endl;
    cout << "(18)   !N           执行倒数第N次执行的命令                    " << endl;
    cout << "(19)   mkdir        新建一个目录                         " << endl;
    cout << "(20)   rmdir        删除一个目录                          " << endl;
    cout << "(21)   help         输出实现的各个命令的功能                " << endl;
    cout << "(22)   exit         退出Shell                              " << endl;
    cout << "(23)   &            在命令后加上&使得命令在新的子进程中运行       " << endl;
}
