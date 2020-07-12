//
// Created by fanqiang on 2020/3/12.
// 使用内核中已经存在的信号对象
// kill -l查看内核中支持多少种信号
//
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void myfun(int signum) {
    cout << "myfun:signum" << signum << endl;
    int i = 0;
    while (i < 6) {
        i++;
        sleep(1);
        cout << "myfun:i:" << i << endl;
    }
}

int main() {
//    kill(pid,9/*信号*/);
    signal(SIGALRM, myfun);//接收到14的信号后，执行myfun
    // signal(14, SIG_IGN);//接收到14的信号后,忽略
    // signal(14, SIG_DFL);//接收到14的信号后，安装默认方式处理
    cout << "before" << endl;
    alarm(5);//10s之后终止当前进程
    cout << "after" << endl;
    int i = 0;
    while (i < 20) {
        i++;
        sleep(1);
        cout << "main:i:" << i << endl;
    }
    return 0;
}

