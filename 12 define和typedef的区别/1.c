#include <stdio.h>

typedef char *P_CHAR;

int main()
{
    // 编译器将 P_CHAR 理解为一个完整的类型：(char*)
    // 因此 p1 和 p2 都是 char* 类型的指针
    P_CHAR p1, p2;

    p1 = "Hello"; // 正确
    p2 = "World"; // 正确

    printf("Size of p1: %zu\n", sizeof(p1)); // 在64位系统上通常是8
    printf("Size of p2: %zu\n", sizeof(p2)); // 在64位系统上通常也是8

    return 0;
}