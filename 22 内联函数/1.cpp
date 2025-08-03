#include <iostream>
using namespace std;

inline int add(int a, int b) { return a + b; }
int main()
{
    int a = add(1, 2);
    cout << a << endl;
    cout << add(1, 2) << endl;
    return 0;
}