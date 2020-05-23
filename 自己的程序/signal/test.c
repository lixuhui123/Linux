#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int a = 1;
void sigcb(int signum)
{
    a = 0;
    printf("a=%d\n", a);
}
int main()
{
    signal(SIGINT, sigcb);
    while(a) {
    }
    return 0;
}
