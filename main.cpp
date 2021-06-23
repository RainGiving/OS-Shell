#include "myShell.h"

int main()
{
    //登录注册部分
    User user;
    int login_choose;
    cout << "初次使用Shell，请选择 1.使用已有账号登录 2.注册新账号并登录" << endl;
    cin >> login_choose;
    if (login_choose == 1)
        user.Login();
    else
        user.Signup();

    getchar(); //吸收最后一个换行符
    historyCheck();
    //命令选择、执行部分
    while (true)
    {
        string cmd[3] = {" ", " ", " "};
        cout << endl;
        cout << "Oh_My_Shell>";
        int i = 0;
        while (cin >> cmd[i++])
            if (cin.get() == '\n') //检测到回车时停止输入命令
                break;

        deal_cmd(cmd);
    }
    return 0;
}
