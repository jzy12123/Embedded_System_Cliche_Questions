//函数重载示例
#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int add(int a, int b, int c)
{
    return a + b + c;
}

int main()
{
    cout << add(1, 2) << endl;
    cout << add(1, 2, 3) << endl;
    return 0;
}