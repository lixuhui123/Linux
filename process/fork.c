#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    printf("*******\n");
    pid_t pid = fork();
    if (pid > 0) {
        sleep(5);
        exit(0);
    }
    while(1) {
        printf("--------\n");
        sleep(1);
    }
    return 0;
}
