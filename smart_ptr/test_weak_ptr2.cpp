#include <iostream>
#include <memory>

class Woman;

/**
 * 在 Man 类内部会引用一个 Woman，Woman 类内部也引用一个 Man。当一个 man 和一个 woman 是夫妻的时候，他们直接就存在了相互引用问题。
 * man 内部有个用于管理wife生命期的 shared_ptr 变量，也就是说 wife 必定是在 husband 去世之后才能去世。同样的，woman 内部也有一个管
 * 理 husband 生命期的 shared_ptr 变量，也就是说 husband 必须在 wife 去世之后才能去世。这就是循环引用存在的问题：husband 的生命期
 * 由 wife 的生命期决定，wife 的生命期由 husband 的生命期决定，最后两人都死不掉，违反了自然规律，导致了内存泄漏。
 *
 * 弱指针可以打破这种循环，因为它不会增加引用计数，但是在需要的时候又可以通过lock获得shared_ptr来使用
 */

class Man {
private:
//    std::weak_ptr<Woman> _wife;
    std::shared_ptr<Woman> _wife;
public:
    void setWife(std::shared_ptr<Woman> woman) {
        _wife = woman;
    }

    ~Man() {
        std::cout << "kill man\n";
    }
};

class Woman {
private:
//    std::weak_ptr<Man> _husband;
    std::shared_ptr<Man> _husband;
public:
    void setHusband(std::shared_ptr<Man> man) {
        _husband = man;
    }

    ~Woman() {
        std::cout << "kill woman\n";
    }
};

int main(int argc, char **argv) {
    std::shared_ptr<Man> m(new Man());
    std::shared_ptr<Woman> w(new Woman());
    if (m && w) {
        m->setWife(w);
        w->setHusband(m);
    }
    std::cout << "man:" << m.use_count() << " woman:" << w.use_count() << std::endl;
    return 0;
}

