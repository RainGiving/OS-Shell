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
#define MAX 5000
using namespace std;

void deal_cmd(string[]);
void HGNB(string[]);
void history(string[]);
void writeHistory(string[]);
void historyCheck();
void cp(string[]);
// void ls(string[]);

const int maxFileSize = 10000; //最大注册数目
int scount = 0;                //当前已注册数目
int history_count = 0;         //历史记录条数

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

void deal_cmd(string cmd[])
{
    if (cmd[0] == "repeat")
    {
        HGNB(cmd);
        writeHistory(cmd);
    }
    else if (cmd[0] == "history")
    {
        history(cmd);
        writeHistory(cmd);
    }
    // else if (cmd[0] == "ls")
    //     ls(cmd);
    else if (cmd[0] == "cp")
    {
        cp(cmd);
        writeHistory(cmd);
    }
    else if (cmd[0] == "exit")
    {
        cout << "Exit Successfully" << endl;
        writeHistory(cmd);
        exit(0); //退出程序
    }
    else
    {
        cout << "Invalid command";
    }
}

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

void history(string cmd[])
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
    if (history_count < 10) //如果记录小于等于10条，则全部打印
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
