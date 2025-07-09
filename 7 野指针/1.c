#include <stdio.h>
#include <malloc.h>

int main()
{
    //? 1 野指针，因为没有赋值
    int *p;

    //? 2 野指针，释放后没有指向NULL，也就没办法再调用了
    char *p2 = malloc(10);
    free(p2);
    // p2 = NULL;

    //? 3 当指针越界的情况下
    int arr[10] = {0};
    p = &arr[10];
    printf("%d\n", *p); // 打印出随机值，因为越界了
    return 0;
}