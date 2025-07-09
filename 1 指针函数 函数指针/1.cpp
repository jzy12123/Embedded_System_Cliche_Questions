#include <stdio.h>
#include <stdlib.h>
/*
 * 函数指针和指针函数
 */
int sum(int a, int b)
{
    return a + b;
}

// *指针函数，一个函数，他的返回值是指针类型
int *p_sum(int a, int b)
{
    int *result = (int *)malloc(sizeof(int));
    if (result != NULL)
    {
        *result = a + b;
    }
    return result;
}
int *p_sum_s(int a, int b)
{
    static int result; // 静态变量，生命周期持续到程序结束
    result = a + b;
    return &result;
}
int main()
{
    // *函数指针
    int (*p)(int, int) = sum;

    p = sum; // 或者这样也行

    printf("%d\n", p(1, 2));      // 3
    printf("%d\n", sum(1, 2));    // 3
    printf("%d\n", (*p)(1, 2));   // 3
    printf("%d\n", (*sum)(1, 2)); // 3

    // *指针函数
    //动态内存分配，用完记得释放内存
    int *p_sum_ptr = p_sum(1, 2); // 因为函数返回值是个指针，所有这里定义一个指针变量
    if (p_sum_ptr != NULL)
    {
        printf("p_sum_ptr = %d\n", *p_sum_ptr); // 输出3
        free(p_sum_ptr);            // 记得释放内存
    }

    //静态使用
    p_sum_ptr = p_sum_s(1,2);
    printf("p_sum_ptr_s = %d\n", *p_sum_ptr); // 输出3
    return 0;
}