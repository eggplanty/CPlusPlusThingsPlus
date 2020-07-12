/*
* MIT Licence
* Copyright 2017 @CodesBay
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the
* Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions :

* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
* ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
* THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
* visit https://github.com/9lean/CplusPlus_SmartPointer for description of the code
*/

#include <iostream>
#include <memory>

using namespace std;

// 打印构造函数和析构函数，帮助追踪内存分配和释放

class Sample {
public:
    Sample() { cout << "Sample Constructor" << endl; }

    ~Sample() { cout << "Sample Destructor" << endl; }

    void publicFn() { cout << "This is public function of class" << endl; }
};


// 返回一个shared_ptr对象
shared_ptr<Sample> AFunc() {
    cout << "Enter AFunc" << endl;
    shared_ptr<Sample> sp(new Sample());
    cout << "Exit AFunc" << endl;
    return sp;
}

// 返回一个普通指针对象
Sample *BFunc() {
    cout << "Enter AFunc" << endl;
    Sample *sp = new Sample();
    cout << "Exit AFunc" << endl;
    return sp;
}

// 测试确认shared_ptr确实释放了内存，并且不用显式调用delete
void TestSharedPtr_ReleaseMemory() {
    shared_ptr<Sample> sp(new Sample());
    sp->publicFn();
    return;
}

// 测试引用计数
void TestSharedPtr_ReferenceCount() {
    cout << "Enter TestSharedPtr_ReferenceCount" << endl;
    shared_ptr<Sample> retSp = AFunc();
    cout << "Exit TestSharedPtr_ReferenceCount" << endl;
    return;
}

// 测试智能使用引用计数
void TestSharedPtr_ReferenceCount_Assignments() {
    cout << "Enter TestSharedPtr_ReferenceCount_Assignments" << endl;
    AFunc();
    cout << "Exit TestSharedPtr_ReferenceCount_Assignments" << endl;
    return;
}

// 普通指针不显式调用delete，则内存不会被释放
void TestSharedPtr_ReleaseMemory2() {
    Sample *p = new Sample();
    p->publicFn();
    return;
}

// 对比普通指针引用
void TestSharedPtr_ReferenceCount2() {
    cout << "Enter TestNomaldPtr_ReferenceCount2" << endl;
    Sample *retSp = BFunc();
    cout << "Exit TestNomaldPtr_ReferenceCount2" << endl;
    return;
}

// 对比使用普通指针引用
void TestSharedPtr_ReferenceCount_Assignments2() {
    cout << "Enter TestNomaldPtr_ReferenceCount_Assignments" << endl;
    BFunc();
    cout << "Exit TestNomaldPtr_ReferenceCount_Assignments2" << endl;
    return;
}

// 测试 std::move 用于 shared_ptr
void TestSharedPtr_Move() {
    cout << "Enter TestSharedPtr_Move" << endl;
    shared_ptr<Sample> p(new Sample());
    cout << "p1:" << p.use_count() << endl;
    shared_ptr<Sample> p2 = move(p);
    cout << "aflter move p1:" << p.use_count() << " p2:" << p2.use_count() << endl;
    cout << "Exit TestSharedPtr_Move" << endl;
    return;
}

// 测试内建的 make_shared 函数来创建 shared_ptr
void TestSharedPtr_MakeShared() {
    shared_ptr<Sample> sp = make_shared<Sample>();
    return;
}

int main() {
    TestSharedPtr_ReleaseMemory();
    cout << "------------------------------------------------" << endl;
    TestSharedPtr_ReferenceCount();
    cout << "------------------------------------------------" << endl;
    TestSharedPtr_ReferenceCount_Assignments();
    cout << "------------------------------------------------" << endl;
    TestSharedPtr_MakeShared();

    cout << "================================================" << endl;
    TestSharedPtr_ReleaseMemory2();
    cout << "------------------------------------------------" << endl;
    TestSharedPtr_ReferenceCount2();
    cout << "------------------------------------------------" << endl;
    TestSharedPtr_ReferenceCount_Assignments2();
    cout << "------------------------------------------------" << endl;
    TestSharedPtr_Move();
    cout << "================================================" << endl;
    return 0;
}
