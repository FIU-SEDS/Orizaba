//definitions go in cpp
#include <stdio.h>

#define DEBUG 1

void debug_print(char *str);

int main()
{
    debug_print(" ");
    return 0;
}

void debug_print(char *str)
{
    #if DEBUG == 1
    printf("%s\n", str);
    #endif
    return;
}