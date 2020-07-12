#include <iostream>
#include <string>

using namespace std;

const int MSIZE = 3;

/**
 * 参考网址
 * https://blog.csdn.net/weixin_44794401/article/details/103606797?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase
 */
template<typename T>
class memorypool//通用内存池类
{
public:
    //返回值不能为memorypool<T>，如果直接以类作为返回值，会有临时对象的生成，这不符合单例模式
    //普通成员方法依赖于对象调用，但Get（）就是为了生成对象而使用的，而没有对象又无法调用，
    //所以将其定义为static，这样就属于对象所共享的了，可以不依赖对象进行调用，从而生成唯一对象
    static memorypool<T> *Get() {
        return &mepool;//直接返回已生成好的对象
    }

    void *alloc(size_t size)//提供开辟内存的接口，size为数据的总大小
    {
        if (pool == NULL)//如果为NULL则没有内存了，需要重新开辟
        {
            //+4是管理内存池的指针域大小，MSIZE是代表开辟的大内存块被分成几个这样小内存
//            pool = (Node * ) new char[(size + 4) * MSIZE]();
            pool = (Node *) malloc((size + 4) * MSIZE);

            Node *pcur = pool;
            for (pcur; pcur < pool + MSIZE - 1; pcur++)//该内存池管理本人用的是类似静态链表的结构管理
            {
                pcur->next = pcur + 1;
            }
            pcur->next = NULL;
        }
        Node *mb = pool;
        pool = pool->next;
        return mb;
        //返回一部分内存作为使用，pool指向未使用部分
    }

    void dealloc(void *ptr)//提供释放内存的接口
    {
        if (ptr == NULL) {
            return;
        }
        Node *p = (Node *) ptr;
        p->next = pool;
        pool = p;
        // pool 是当前第一个还未使用的内存区域
        //将归还的内存指针域指向pool原先所指向的部分，pool再指向该部分，表明该部分未使用
        //具体释放内存等最后释放内存池内存时一起释放
    }

private:
    //将析构函数与构造函数放在私有，杜绝其生成其他对象，仅通过接口Get（）生成唯一对象
    memorypool() {}

    memorypool(const memorypool<T> &);

    class Node//内存池中的内存布局结构
    {
    public:
        Node(T val) : mdata(val), next(NULL) {}

    public:
        T mdata;//存储数据部分
        Node *next;//管理内存块的指针域部分
    };

    static Node *pool;//指向内存池未使用部分的指针，共用一个指向内存块的指针即可

    static memorypool<T> mepool;
    //生成唯一对象，static是针对于不同地方仅生成一个内存池对象就可以了
    //这里只是一个声明，定义在类外
};

template<typename T>
typename memorypool<T>::Node *memorypool<T>::pool = NULL;
//因为memorypool是个模板，系统不知道Node是变量还是类型，所以要加个typename指明这是个类型
template<typename T>
memorypool<T> memorypool<T>::mepool;//调用构造函数生成唯一对象


class Student//学生类，用以测试内存池的调用
{
public:
    Student(string name, int age) : mname(name), mage(age) {}

    void *operator new(size_t size)//new的重载函数
    {
        return mpool->alloc(size);//返回开辟内存的接口并调用
    }

    void operator delete(void *ptr)//delete的重载函数
    {
        mpool->dealloc(ptr);//调用释放内存的接口
    }

    void print() {
        cout << mname << " " << mage << endl;
    }

private:
    string mname;
    int mage;
    static memorypool<Student> *mpool;
    //指向内存池的指针，针对于不同对象仅用一个内存池管理即可，故用static设为对象共享
};

memorypool<Student> *Student::mpool = memorypool<Student>::Get();
//调用接口生成唯一内存池对象

int test() {
//测试用例
    Student *st1 = new Student("asd", 12);
    Student *st2 = new Student("rtfyhd", 18);
    Student *st3 = new Student("sdsf", 23);
    Student *st4 = new Student("rtey", 22);
    st1->print();
    st2->print();
    st3->print();
    st4->print();

    return 0;
}