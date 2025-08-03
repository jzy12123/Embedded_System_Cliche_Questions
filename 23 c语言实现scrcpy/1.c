#include <stdio.h>

// 实现字符串拷贝
char *my_strcpy(char *dest, const char *src)
{
    // 定义指针指向目的地址
    char *original_dest = dest;        
    // 将源的值不断赋值给目的地址，直到遇到'\0'结束循环
    while ((*dest++ = *src++) != '\0') 
    {
        // 这里不做任何事情，只是递增指针和赋值操作
    }
    return original_dest;
}

int main()
{
    char src[] = "Hello, World!";
    char dest[20];
    my_strcpy(dest, src);
    printf("%s\n", dest); // 输出：Hello, World!
}