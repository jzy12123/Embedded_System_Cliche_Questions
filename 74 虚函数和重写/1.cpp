#include <iostream>
using namespace std;

class Base
{
public:
    virtual void show() // 虚函数
    {
        cout << "Base show" << endl;
    }
    void display() // 非虚函数
    {
        cout << "Base display" << endl;
    }

    virtual ~Base() {}; // 虚析构函数，为了防止内存泄漏
};

class Derived : public Base
{
public:
    void show() override
    {
        cout << "Derived show" << endl;
    }
    void display()
    {
        cout << "Derived display" << endl;
    }
};
int main()
{
    Base *b = new Derived(); // 使用基类的指针 指向子类
    b->show();
    b->display();

    delete b;
    return 0;
}