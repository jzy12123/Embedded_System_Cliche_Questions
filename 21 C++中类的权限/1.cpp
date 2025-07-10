#include <iostream>
using namespace std;

class jzy
{
private:
    int b;

protected:
    int c;

public:
    int a;

    void fun()
    {
        b = 20;
        c = 30;
        cout << b << " " << c;
    }
};
int main()
{
    jzy jzy;
    jzy.a = 10;
    jzy.fun();
    cout << jzy.a;
    return 0;
}