#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>

int main()
{
    int epfd, nfds;
    char buf[256];
    struct epoll_event event, events[5];
    epfd = epoll_create(1);
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN;  // LT是默认模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    while (1) {
        nfds = epoll_wait(epfd, events, 5, -1);
        int i;
        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
                // LT模式，如果数据读完了，则下次调用epoll_wait会被阻塞，读事件不被触发
                read(STDIN_FILENO, buf, sizeof(buf));
                printf("hello world\n");
            }
        }
    }
}
