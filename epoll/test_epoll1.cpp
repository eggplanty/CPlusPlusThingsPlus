#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>

/**
 * 水平触发
 * 1. 对于读操作
 * socket接收缓冲区不为空 有数据可读 读事件一直触发
 * 2. 对于写操作
 * socket发送缓冲区不满 可以继续写入数据 写事件一直触发
 *
 * 边缘触发
 * 1. 对于读操作
 * （1）当缓冲区由不可读变为可读的时候，即缓冲区由空变为不空的时候。
 * （2）当有新数据到达时，即缓冲区中的待读数据变多的时候。
 * （3）当缓冲区有数据可读，且应用进程对相应的描述符进行EPOLL_CTL_MOD 修改EPOLLIN事件时。
 * 2. 对于写操作
 * （1）当缓冲区由不可写变为可写时。
 * （2）当有旧数据被发送走，即缓冲区中的内容变少的时候。
 * （3）当缓冲区有空间可写，且应用进程对相应的描述符进行EPOLL_CTL_MOD 修改EPOLLOUT事件时。
 *
 * 当用户输入一组字符，这组字符被送入缓冲区，因为缓冲区由空变成不空，所以ET返回读就绪，输出”hello world”。
 * 之后再次执行epoll_wait，但ET模式下只会通知应用进程一次，故导致epoll_wait阻塞。
 * 如果用户再次输入一组字符，导致缓冲区内容增多，ET会再返回就绪，应用进程再次输出”hello world”。
 * 如果将上面的代码中的event.events = EPOLLIN | EPOLLET;改成event.events = EPOLLIN;，即使用LT模式，则运行程序后，会一直输出hello world。
 */
int main()
{
    int epfd, nfds;
    struct epoll_event event, events[5];
    epfd = epoll_create(1);
    event.data.fd = STDIN_FILENO;
    // event.events = EPOLLIN | EPOLLET;
    event.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    while (1) {
        nfds = epoll_wait(epfd, events, 5, -1);
        int i;
        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
                printf("hello world\n");
            }
        }
    }
}
