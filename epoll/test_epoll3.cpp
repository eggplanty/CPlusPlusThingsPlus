#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>

/**
 * 实验3使用ET模式，但是每次读就绪后都主动对描述符进行EPOLL_CTL_MOD 修改EPOLLIN事件，由上面的描述我们可以知道，会
 * 再次触发读就绪，这样就导致程序出现死循环，不断地在屏幕中输出”hello world”。但是，如果我们将EPOLL_CTL_MOD 改
 * 为EPOLL_CTL_ADD，则程序的运行将不会出现死循环的情况。
 *
 */

int main()
{
    int epfd, nfds;
    struct epoll_event event, events[5];
    epfd = epoll_create(1);
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    while (1) {
        nfds = epoll_wait(epfd, events, 5, -1);
        int i;
        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
                printf("hello world\n");
                event.data.fd = STDIN_FILENO;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, STDIN_FILENO, &event); // 主动修改事件属性
            }
        }
    }
}
