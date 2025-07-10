#include <stdio.h>

int main()
{
    int a = 10;
    int b = 20;

    // 常量指针
    const int *ptr1 = &a;
    // *ptr1 = 30; // 错误：不能通过ptr1修改a的值
    ptr1 = &b; // 正确：ptr1的指向可以改变

    // 指针常量
    int *const ptr2 = &a;
    *ptr2 = 30; // 正确：可以通过ptr2修改a的值
    // ptr2 = &b; // 错误：ptr2的指向不能改变

    printf("a = %d, b = %d\n", a, b);
    printf("*ptr1 = %d, *ptr2 = %d\n", *ptr1, *ptr2);

    return 0;
}