#include <stdio.h>

#define MAX_SIZE 100
int main()
{
    // 定义一个整型数组
    int arr[MAX_SIZE];
    int n = 5; // 实际元素个数

    // 初始化数组
    for (int i = 0; i < n; i++)
    {
        arr[i] = i + 1; // 1, 2, 3, 4, 5
    }

    // 访问元素
    printf("arr[3] %d\n", arr[2]);

    // 插入元素（在末尾）
    if (n < MAX_SIZE)
    {
        arr[n++] = 6;
    }

    // 删除元素（删除最后一个）
    if (n > 0)
    {
        n--;
    }

    return 0;
}