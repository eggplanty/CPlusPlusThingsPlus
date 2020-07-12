//
// Created by fanqiang on 2020/3/13.
//
// 共享内存读完之后，内容还存在
// 共享内存创建之后，一直存在于内核中，直到被删除或系统关闭
//
#include <stdio.h>
#include <iostream>
#include <sys/shm.h>

using namespace std;

int main() {
    int key = ftok("./a.c", 'a');
    //第一个参数：文件路径,文件必须存在
    //第二个参数：一个字符
    //以ftok创建的key，可以实现非亲缘关系进程间通信

    if (key == -1) {
        cout << "key 创建失败" << endl;
        return -1;
    }
    cout << "共享内存key:" << key << endl;
    int shmid = shmget(key, 128, IPC_CREAT/*以ftok创建的key，需要这个参数*/ | 0777);
//    int shmid = shmget(IPC_PRIVATE, 128, 0777);//在内核中生成共享内存的对象；相当于缓存
    //第一个参数：key,这中方式创建的key永远是0,只能实现有亲缘关系进程间通信
    //第二个参数：共享内存大小
    //第三个参数：访问权限

    if (shmid == -1) {
        cout << "共享内存创建失败" << endl;
        return -1;
    }
    cout << "共享内存创建成功" << endl;
    char *shmat_ret = (char *) shmat(shmid, NULL, 0);
    //为了方便用户空间对共享内存的操作，使用地址映射      否则的话每次都需要进入内核再进行操作
    //实现将内核中的内存映射到用户空间；在用户空间对内存的操作都会映射到内核空间中
    //可以将共享内存映射到用户空间地址中
    //第二个参数为映射到的地址，NULL为系统自动完成映射
    //第三个参数：默认是0，标示共享内存可读写；SHM_RDONLY共享内存只读
    //返回值，NULL失败，否则为映射后的地址；

    if (shmat_ret == NULL) {
        cout << "内存映射失败：" << endl;
        return -3;
    }
    fgets(shmat_ret, 128, stdin);
    //写内存
    //第二个参数标示大小
    //第三个参数：从键盘读入数据

    cout << "共享内存内容1："<< shmat_ret << endl;
    cout << "共享内存内容2："<< shmat_ret << endl;// 写入共享内存的数据会一致存在
    shmdt(shmat_ret);
    //将用户空间的内存释放

    shmctl(shmid,IPC_RMID,NULL);
    //将内核空间的内存释放
    //第二个参数：IPC_STAT获取内核空间属性
    //          IPC_SET设置内核空间属性
    //          IPC_RMID删除对象
    //第三个参数：指定IPC_STAT或IPC_SET时的结构体



    return 0;
}

