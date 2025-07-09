# include <stdio.h>
int main(){
    int *a;
    float *b;
    char *c;
    //现在我们打印出三个指针的大小：
    printf("%d\n", sizeof(a)); // 8
    printf("%d\n", sizeof(b)); // 8
    printf("%d\n", sizeof(c)); // 8
    // ?结果都是8，说明指针的大小是和编译器相关的，64位编译器下指针大小为8字节，32位编译器下为4字节。
    return 0;
}