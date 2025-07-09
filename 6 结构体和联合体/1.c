#include <stdio.h>

typedef union
{
    /* data */
    int a;
    float b;
    char c;
    double d[10]; // 数组所占空间大小是固定的，不会因为元素个数变化而改变。所以这里的大小是固定的，而且是最大的那个数据类型的大小。
} Data;           // 共用体的大小是其最大成员的大小，为8×10 = 80。

typedef struct
{
    char name[20]; //! char占用一个字节，所以20个字符占用20个字节。不满足字节对齐，所以实际应该占用了24个字节。
    Data data;     // data所占空间大小为80,满足字节对齐
} Student;         // 结构体的大小是其所有成员大小的总和，为80+24=104。

int main()
{
    printf("sizeof(Data) = %lu\n", sizeof(Data));
    printf("sizeof(Student) = %lu\n", sizeof(Student));
    return 0;
}