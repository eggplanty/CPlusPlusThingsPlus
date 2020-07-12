#include<sys/types.h>
#include<sys/stat.h>
#include <sys/mman.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

struct myinfo {
    int sex;
    int aa;
};

struct myinfo *get_mmap(size_t length);

int main() {

    size_t length = 8;
    int pid = fork();
    if (pid == 0) {
        struct myinfo *mm = get_mmap(length);
        cout << "child mm:" << mm << endl;
        mm->sex = 2;
        mm->aa = 3;
        int mun_ret = munmap(mm, length);//即使不释放，内容也会在文件中修改
        if (mun_ret == -1) {
            cout << "munmap error" << endl;
        }
    } else {
        sleep(4);
        struct myinfo *mm = get_mmap(length);

        cout << "mm:" << mm << endl;
        cout << "mm.aa:" << mm->aa << endl;
        cout << "mm.sex:" << mm->sex << endl;
        int mun_ret = munmap(mm, length);
        if (mun_ret == -1) {
            cout << "munmap error" << endl;
        }
    }
    return 0;
}

struct myinfo *get_mmap(size_t length) {
    int fd = open("mmaptest", O_CREAT | O_RDWR, 00777); // 需要指定权限，默认只有执行权限
    ftruncate(fd,length);
    struct myinfo *mm = (struct myinfo *) mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    //第二个参数，内存映射的大小
    //第五个参数，文件描述符，-1标示匿名文件描述符；
    //第六个参数，偏移量
    close(fd);
    if (mm == MAP_FAILED) {
        cout << "mmap error" << endl;
        return NULL;
    }
    return mm;
}
