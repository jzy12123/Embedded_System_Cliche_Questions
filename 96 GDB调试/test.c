#include <stdio.h>

int calculate_sum(int n)
{
    int sum = 0;
    for (int i = 1; i <= n; i++)
    {
        sum += i;
    }
    return sum; // 我们想在这里设置断点
}

int main()
{
    int result = calculate_sum(5);
    printf("Sum is: %d\n", result);
    return 0;
}