#include <unistd.h>
#include <iostream>
//
// Created by fanqiang on 2020/3/12.
// 参考 https://www.bilibili.com/video/av76289534?p=1

//
// 1.无名管道
// 管道是特殊的文件
// 底层用队列实现
// 管道中内容，读完就删除了
// 缺点：只能在父子进程或有亲缘关系的进程中使用：
//
void unname_pipe_1();

void unname_pipe_2();

using namespace std;

int main() {
    unname_pipe_1();
    unname_pipe_2();
}

//无名管道测试：父子进程使用管道通信
void unname_pipe_2() {
    int fd[2];
    int ret = pipe(fd);
    if (ret == -1) {
        cout << "pipe create failed" << endl;
        return;
    }
    pid_t id = fork();//在fork的时候，子进程会拿到父进程的内存拷贝，同时子进程和父进程都可以拿到之前创建的管道并访问文件描述符；
    if (id == 0) {//子进程
        int size = 200;
        char *rec = new char[size]();
        read(fd[0], rec, size);
        cout << "read from parent:" << rec << endl;
    } else {
        string string1 = "from parent";
        write(fd[1], string1.data(), string1.size());
    }
}

// 无名管道测试：单进程使用无名管道读写
void unname_pipe_1() {
    int fd[2];
    int result = pipe(fd);//创建无名管道；使用fd[0]文件描述符读，使用fd[1]文件描述符写；
    if (result == -1) {
        cout << "管道创建失败" << endl;
        return;
    }
    cout << "管道创建成功" << endl;
    char *str = const_cast<char *>("this is my world!");
    ssize_t write_size = write(fd[1], str, sizeof(str));
    cout << "写大小：" << write_size << endl;


    size_t read_size = 200;
    char *str_result = new char[read_size]();
    ssize_t r = read(fd[0], str_result, read_size);//如果管道没有内容阻塞；进程处于休眠状态；
    cout << "读大小：" << r << endl;

    int a = 1;
    int index = 0;
    while (1) {//测试管道大小；管道如果写满了也会阻塞；进程处于休眠
        write(fd[1], &a, 1);
        cout << "当前管道写入字节数" << ++index << endl;//测试结果为65536个字节
    }

    close(fd[0]);//关闭读文件描述符
    close(fd[1]);//关闭写文件描述符
}
