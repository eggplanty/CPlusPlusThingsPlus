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

// 使用构造函数和析构函数，在内存分配和释放时打印信息
class Sample {
public:
    int a = 1;

    Sample() { cout << "Sample Constructor" << endl; }

    ~Sample() { cout << "Sample Destructor" << endl; }

    void publicFn() { cout << "This is public function of class" << endl; }
};

// 测试确认unique_ptr确实释放了内存，并且不用显式调用delete
void TestUniquePtr_ReleaseMemory() {
    unique_ptr<Sample> up(new Sample{});
    up->publicFn();
    return;
}

// 测试unique_ptr无法通过直接赋值转换所有权，这在auto_ptr中是可行的
void TestUniquePtr_NoOwnershipTrasfer() {
    unique_ptr<Sample> up(new Sample);
    // 下面这行代码会出现编译错误
//    unique_ptr<Sample> upTrans = up;

//    auto_ptr<Sample> down(new Sample);
//    auto_ptr<Sample> downTrans = down;
    return;
}

// 测试通过 std::move 转换 unique_ptr 的所有权，所有权转移之后，原指针成为空指针
void TestUniquePtr_MoveOwnershipTrasfer() {
    unique_ptr<Sample> up(new Sample);
    cout << up->a << endl;
    cout << (up == NULL) << endl;

    unique_ptr<Sample> upTrans = std::move(up);
    cout << (up == NULL) << endl;
    cout << upTrans->a << endl;

    return;
}

// 辅助函数，返回一个unique_ptr
unique_ptr<Sample> AFunc() {
    cout << "Enter AFunc" << endl;
    unique_ptr<Sample> up(new Sample{});
    cout << "Exit AFunc" << endl;
    return up;
}

// 测试 unique_ptr 从函数返回的时候不分享所有权
void TestUniquePtr_ReturnFunction() {
    cout << "Enter TestUniquePtr_ReturnFunction" << endl;
    unique_ptr<Sample> retSp = AFunc();
    cout << "Exit TestUniquePtr_ReturnFunction" << endl;
    return;
}

// 如果没有人承接所有权，则 unique_ptr 会释放内存
void TestUniquePtr_ReturnNoOwnership() {
    cout << "Enter TestUniquePtr_ReturnNoOwnership" << endl;
    AFunc();
    cout << "Exit TestUniquePtr_ReturnNoOwnership" << endl;
    return;
}

// 内建的 make_unique 来创建 unique_ptr， 只有高于C++14才可用
void TestUniquePtr_MakeUnique() {
    unique_ptr<Sample> sp = make_unique<Sample>();
    return;
}

int main() {
    TestUniquePtr_ReleaseMemory();
    cout << "------------------------------------------------" << endl;
    TestUniquePtr_MoveOwnershipTrasfer();
    cout << "------------------------------------------------" << endl;
    TestUniquePtr_ReturnFunction();
    cout << "------------------------------------------------" << endl;
    TestUniquePtr_ReturnNoOwnership();
    cout << "------------------------------------------------" << endl;
    TestUniquePtr_MakeUnique();
    return 0;
}
