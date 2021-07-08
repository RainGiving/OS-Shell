#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

int main()
{
    // string str = "ls -l yyds", word;
    // stringstream ss(str);
    // while (ss >> word)
    //     cout << word << endl;
    const string cmd = "!3";
    string s_num;
    for (int i = 1; i < cmd.size(); i++)
        s_num += cmd[i];
    int num = stoi(s_num);
    cout << num << endl;
    return 0;
}