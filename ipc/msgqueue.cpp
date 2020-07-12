//
// Created by fanqiang on 2020/3/13.
// 消息队列
// 读完内容之后，消息就删除了
//
#include <sys/msg.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;
struct msg_struct {
    long type;//必须包含消息类型
    char data[124];
};

int main() {
    int type = 100;
    int msgid = msgget(IPC_PRIVATE, 0777);
    //在内核空间创建消息队列
    if (msgid == -1) {
        cout << "消息队列创建失败" << endl;
        return -1;
    }
    struct msg_struct msgStruct, recStruct;
    msgStruct.type = type;
    cout << "send to msg queue：" << endl;
    fgets(msgStruct.data, 124, stdin);

    int send_ret = msgsnd(msgid, (void *) &msgStruct/*传递的是整个结构体*/, strlen(msgStruct.data), 0);
    //第二个参数标示除了消息类型之外的大小
    //第四个参数标示是否阻塞；IPC_NOWAIT:函数立刻返回；0阻塞；
    if (send_ret == -1) {
        cout << "send error" << endl;
        return -2;
    }

    memset(recStruct.data, 0, 124);
    ssize_t rec_len = msgrcv(msgid, (void *) &recStruct/*传递的是整个结构体*/, 124, type, 0);
    //第五个参数标示消息类型
    //第六个参数标示是否阻塞；
    //                  IPC_NOWAIT:函数立刻返回；
    //                  0:阻塞；
    cout << "读到的内存大小：" << rec_len << endl;
    cout << "读到的内存：" << recStruct.data << endl;

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}

