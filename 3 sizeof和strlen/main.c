#include <stdio.h>
#include <string.h>
int main()
{
    printf("%d %d\r\n", sizeof("hello"), strlen("hello")); // 6 5
    printf("%d %d\r\n", sizeof("\0"), strlen("\0"));       // 2 0
    return 0;
}