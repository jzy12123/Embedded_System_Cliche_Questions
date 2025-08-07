// 面向对象三大特征，封装、继承多态，使用animal、cat和dog做例子
#include <iostream>
using namespace std;

class Animal
{
protected:
    string name;

public:
    Animal(string n) : name(n) {} // 封装，私有成员变量name，公有构造函数Animal(string n) : name(n){}
    void eat()
    {
        cout << "Animal is eating" << endl;
    }
};

class Cat : public Animal
{
public:
    Cat(string n) : Animal(n) {}
    void eat()
    {
        cout << name << " is eating" << endl;
    }
};

class Dog : public Animal
{
public:
    Dog(string n) : Animal(n) {}
    void eat()
    {
        cout << name << " is eating" << endl;
    }
};

int main()
{
    Animal a("Animal");
    a.eat();

    Cat c("Cat");
    c.eat();

    Dog d("Dog");
    d.eat();

    return 0;
}