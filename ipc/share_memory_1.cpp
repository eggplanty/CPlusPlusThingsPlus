#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

//
// Created by fanqiang on 2020/3/13.
// 父子进程通过共享内存通信
// 如果想实现双向通信，使用两个共享内存
//

using namespace std;

void myfun(int signum) {

}

int main() {
    int shmid = shmget(IPC_PRIVATE, 128, 0777);
    if (shmid == -1) {
        cout << "共享内存创建失败" << endl;
        return -1;
    }
    int i = 0;
    const int num = 3;
    char *p;
    int pid = fork();
    if (pid == 0) {//子进程
        p = (char *) shmat(shmid, NULL, 0);
        if (p == NULL) {
            cout << "子进程内存映射失败" << endl;
            return -2;
        }
        signal(SIGUSR1, myfun);
        while (i < num) {
            pause();
            cout << "子进程读取：" << p << endl;
            kill(getppid(), SIGUSR1);
            i++;
        }
        shmdt(p);
    } else {//父进程
        p = (char *) shmat(shmid, NULL, 0);//映射完毕之后，两个进程就可以**直接**访问这个用户空间的内存；
        if (p == NULL) {
            cout << "父进程内存映射失败" << endl;
            return -2;
        }
        signal(SIGUSR1, myfun);
        while (i < num) {
            cout << "父进程输入内容：" << endl;
            fgets(p, 128, stdin);
            kill(pid, SIGUSR1);
            pause();
            i++;
        }
        shmdt(p);
    }
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

