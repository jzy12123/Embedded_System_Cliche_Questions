
//使用c语言编写一个scrcpy函数
//比较字符串是否相同，每一个字符串都要相同
#include<stdio.h>
int scrcpy(char *s1, char *s2)
{
    while(*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

int main()
{
    char s1[] = "hello";
    char s2[] = "hello";
    if (scrcpy(s1, s2) == 0)
    {
        printf("str same\n");
    }
    else
    {
        printf("str not same\n");
    }
    return 0;
}

