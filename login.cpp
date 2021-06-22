#include "System.h"
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
