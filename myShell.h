//-----------------------------------------------------------头文件-------------------------------------------------
#include <iostream>
#include <fstream>
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
#include <bits/stdc++.h>
using namespace std;
//--------------------------------------------------------------------------------------------------------------------

//---------------------------------------------声明函数和全局变量-------------------------------------------------------
void deal_cmd(string[]);     //识别命令并跳转至各命令对应的函数
void HGNB(string[]);         // 重复赞美豪哥
void history();              // 查看历史命令记录
void writeHistory(string[]); //每次执行命令后将命令保存至history.txt文件
void historyCheck();         //每次开始输入命令前检查history.txt里是否有之前的历史命令，更新history_count
void cp(string[]);           //复制文件1到文件2
void pwd();                 //输出当前工作目录的绝对路径
// void ls(string[]);
void cat(string[]);         //显示文件内容
void cal();                 //显示日历
void date();                //显示当前日期时间
void mv(string[]);          //将文件移动到zhi
//-------------------------------------------------------------------------
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

//---------------------------------------------------实现识别处理命令cmd，并跳转到各命令函数---------------------------------
void deal_cmd(string cmd[])
{
    if (cmd[0] == "repeat")
    {
        HGNB(cmd);
        writeHistory(cmd);
    }

    else if (cmd[0] == "history")
    {
        history();
        writeHistory(cmd);
    }
    // else if (cmd[0] == "ls")
    //     ls(cmd);
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
    }
    else if (cmd[0] == "cal")
    {
        cal();
    }
    else if (cmd[0] == "date")
    {
        date();
    }
    else if (cmd[0] == "mv")
    {
        mv(cmd);
    }
    else
    {
        cout << "Invalid command";
    }
}

//------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------实现重复赞美豪哥的功能-------------------------------------------------
void HGNB(string cmd[])
{
    int times;
    if (cmd[1] == " ")
        times = 10;
    else
        times = stoi(cmd[1], 0, 10);
    for (int i = 0; i < times; ++i)
        cout
            << "豪哥牛逼"
            << " ";
}
//------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------实现查询历史记录功能--------------------------------------------------
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

//-------------------------------------------------------输出当前工作目录的绝对路径-------------------------------------------

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
        perror("fopen err:\n");
    char *buf = (char *)malloc(100);
    while (fgets(buf, 100, fa) != NULL)
    {
        printf("%s", buf);
        memset(buf, 0, 100);
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

//------------------------------------------date显示当前日期-----------------------------------------------------------------
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

//-----------------------------------mv移动文件-----------------------------------------------------------
void mv(string cmd[])
{
    char *cmd1, *cmd2;
    strncpy(cmd1, cmd[1].c_str(), cmd[1].length());
    strncpy(cmd2, cmd[2].c_str(), cmd[2].length());
    FILE *source = fopen(cmd1, "r");
    if (source == NULL)
    {
        perror("file no exsit!\n");
        return;
    }
    else
    {
        struct stat *statbuf = (struct stat *)malloc(sizeof(statbuf));
        stat(cmd2, statbuf);           //判断目标路径是一个普通文件还是一个目录
        if (S_ISDIR(statbuf->st_mode)) //目标是个目录
        {
            int i, j, k = 0;
            //因为目标路径是一个目录，所以默认拷贝一个和源文件名字一样的名字
            //要找到源文件的名字，就要找源路径最后一个'/'后面的字符串就是源文件名！！
            for (i = strlen(cmd1) - 1; i >= 0; i--)
            {
                if (cmd1[i] == '/') //找到了最后一个'/'的下标
                {
                    break; //找到就退出 ，保留'/'下标为i
                }
            }
            char source_file_name[128] = {}; //用来存储源文件的名字
            for (j = i + 1; j < strlen(cmd1); j++)
            { //把源路径最后一个'/'后面的字符串给source_file_name,它就是源文件名字
                source_file_name[k++] = cmd1[j];
            }
            char *cmd22;

            //如果目标路径最后面没有‘/’，则需要加一个'/'
            if (cmd2[strlen(cmd2) - 1] != '/')
            {
                strcat(cmd2, "/");
            }
            //把目标路径和源文件名拼接起来，成为一个目标文件名，并创建打开它
            FILE *target = fopen(strcat(cmd2, source_file_name), "w+");
            while (!feof(source))
            { //把源文件内容全部传给目标文件
                char buf[10] = {};
                fread(buf, 1, 10, source);
                fwrite(buf, 1, 10, target);
            }
            fclose(target);
        }
        else //目标路径是个文件
        {
            FILE *target = fopen(cmd2, "w+");
            while (!feof(source))
            { //把源文件内容全部传给目标文件
                char buf[10] = {};
                fread(buf, 1, 10, source);
                fwrite(buf, 1, 10, target);
            }
            fclose(target);
        }
    }
    remove(cmd1); //删除源文件
    fclose(source);
}