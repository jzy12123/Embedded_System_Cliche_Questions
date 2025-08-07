//智能指针示例
#include <iostream>
#include <memory>
using namespace std;

class Test {
public:
    Test() { cout << "Test 构造函数" << endl; }
    ~Test() { cout << "Test 析构函数" << endl; }
    void show() { cout << "Test::show()" << endl; }
};

int main() {
    // 使用 unique_ptr
    unique_ptr<Test> ptr1(new Test());
    ptr1->show();
    return 0;
}

