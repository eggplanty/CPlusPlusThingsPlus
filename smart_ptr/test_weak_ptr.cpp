/*
* visit https://blog.csdn.net/k346k346/article/details/81478223 for description of the code
*/

#include <iostream>
#include <memory>

using namespace std;

/**
 * weak_ptr 被设计为与 shared_ptr 共同工作，可以从一个 shared_ptr 或者另一个 weak_ptr 对象构造而来。weak_ptr 是为了配合 shared_ptr 而
 * 引入的一种智能指针，它更像是 shared_ptr 的一个助手而不是智能指针，因为它不具有普通指针的行为，没有重载 operator* 和 operator-> ，因此取
 * 名为 weak，表明其是功能较弱的智能指针。它的最大作用在于协助 shared_ptr 工作，可获得资源的观测权，像旁观者那样观测资源的使用情况。观察者意
 * 味着 weak_ptr 只对 shared_ptr 进行引用，而不改变其引用计数，当被观察的 shared_ptr 失效后，相应的 weak_ptr 也相应失效。
 *
 * weak_ptr可以使用expired()函数快速判断use_count是否为0，可以使用lock()函数获得shared_ptr的对象
 */

// 打印构造函数和析构函数，帮助追踪内存分配和释放
class Sample {
public:
    Sample() { cout << "Sample Constructor" << endl; }

    ~Sample() { cout << "Sample Destructor" << endl; }

    void publicFn() { cout << "This is public function of class" << endl; }
};

// 测试创建一个weak_ptr指针
weak_ptr<Sample> Test_create_weak_ptr() {
    cout << "enter Test_create_weak_ptr" << endl;
    shared_ptr<Sample> p(new Sample);
    weak_ptr<Sample> wp = p; // 作为shared_ptr的辅助监控函数, expired可以快速判断是否失效，即use_count==0
    cout << "shared_ptr:" << p.use_count() << " weak_ptr:" << wp.use_count() << " expired:" << wp.expired() << endl;

    shared_ptr<Sample> mp = wp.lock(); // 获得weak_ptr指向的shared_ptr的一个指针
    mp->publicFn();

    p.reset();
    cout << "shared_ptr:" << p.use_count() << " weak_ptr:" << wp.use_count() << " expired:" << wp.expired() << endl;

    return wp;
}


int main() {

    Test_create_weak_ptr();

    return 0;
}
