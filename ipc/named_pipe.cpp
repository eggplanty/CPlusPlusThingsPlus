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


//要测试该例子，需要线运行该main，生成可执行文件之后，运行named_pipe_another.cpp，然后查看输出；
//
using namespace std;


int main() {
    int ret = mkfifo("./namedpipe10",  0777);// 第一个参数标示文件名；
    // 会在当前目录下创建一个namedpipe的管道文件；umask是0777(8进制的 777)
    // 如果不是8进制下的0777，打开管道文件会失败
    if (ret == -1) {
        cout << "有名管道创建失败" << endl;
        // return -1;
    }
    cout << "有名管道创建成功" << endl;

    int open_fd = open("./namedpipe10", O_WRONLY);//第二个参数标示只写
    if (open_fd == -1) {
        cout << "有名管道打开失败" << endl;
        return -1;
    }
    cout << "有名管道打开成功" << endl;
    string content = "from another progress";
    write(open_fd,content.data(),content.size());
    while(1);
    return 0;
}
