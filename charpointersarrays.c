#include <stdio.h>

int main()
{

    char *q = "hello";
    printf("%c", q[4]);
    
    char p[] = "hello";
    p[4] = 'p';
    printf("%c", p[4]);
    return 0;
}