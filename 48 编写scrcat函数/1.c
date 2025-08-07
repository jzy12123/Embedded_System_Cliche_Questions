// 实现scrcat函数，实现字符串的拼接
#include <stdio.h>
#include <string.h>

char *my_strcat(char *dest, const char *src)
{
    // 找到目标字符串的末尾位置
    char *ret = dest;
    while (*dest != '\0')
    {
        dest++;
    }
    // 将源字符串的内容复制到目标字符串的末尾
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    // 在目标字符串的末尾添加一个空字符
    *dest = '\0';
    return ret;
}

int main()
{
    char str1[50] = "Hello";
    const char *str2 = " World!";
    char *result = "0";
    result = my_strcat(str1, str2);
    printf("%s\n", result);
    return 0;
}