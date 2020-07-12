//
// Created by fanqiang on 2020/3/13.
// 消息队列
// 两个进程使用一个消息队列读写操作 - 服务端
//
#include <sys/msg.h>
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

atomic<bool> over(false);

struct mystruct {
    long type;
    char data[128];
};

void read(int msgid, int msgtype) {
    struct mystruct rec;
    while (!over) {
        msgrcv(msgid, (void *) &rec, 128, msgtype, 0);
        cout << "read msgtype:" << msgtype << " content:" << rec.data << endl;
    }
}

void write(int msgid, int msgtype) {
    struct mystruct sendstruct;
    sendstruct.type = msgtype;
    int i = 0;
    while (i < 3) {
        fgets(sendstruct.data, 128, stdin);
        msgsnd(msgid, (void *) &sendstruct, 128, 0);
        i++;
    }
    over = true;
}

int main() {
    int key = ftok("./a.c", 'b');//同样和共享内存一样使用ftok生成key
    if (key == -1) {
        cout << "key created failed" << endl;
        return -1;
    }
    int msgid = msgget(key, IPC_CREAT|0777);
    if (msgid == -1) {
        cout << "msg created failed" << endl;
        return -2;
    }
    int read_type = 100;
    int write_type = 200;
    thread read_thread(read, msgid, read_type);
    thread write_thread(write, msgid, write_type);
    while (!over) {
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}

