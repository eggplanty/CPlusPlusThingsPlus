#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

//
// Created by fanqiang on 2020/3/12.
// 有名管道
// 在文件系统中存在文件节点：管道文件
// 该文件不占磁盘空间
// 调用mkfifo之后，内核会在用户空间生成一个文件名
// 在调用open之后会在内核中创建管道
// 可以实现无亲缘关系进程间通信
//
using namespace std;

int main() {
    int fd = open("./namedpipe10"/*文件名与pipe_name中创建的管道文件一直*/, O_RDONLY);
    if (fd == -1) {
        cout << "管道文件打开失败" << endl;
        return -1;
    }
    cout << "管道文件打开成功" << endl;
    int size = 200;
    char *rec = new char[size]();
    read(fd, rec, size);
    cout << "read from anther progress:" << rec << endl;

}
