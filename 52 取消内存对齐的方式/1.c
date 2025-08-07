#include <stdio.h>

#pragma pack(1) // 取消内存对齐
typedef struct
{
    char a; // 1
    int b;  // 4
    int c;  // 4
} MYSTRUCT;
#pragma pack() // 恢复内存对齐设置

int main()
{
    MYSTRUCT s;
    s.a = 'A';
    s.b = 10;
    s.c = 20;
    printf("%d\n", sizeof(s));
}