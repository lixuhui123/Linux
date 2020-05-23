#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    printf("********%d\n", getpid());
    pid_t pid = fork();
    if (pid  < 0) {
        printf("fork error\n");
        return -1;
    }else if (pid == 0) {
        //这段代码只有子进程能够更运行到，因为在子进程中fork的返回值=0
        printf("this is child!!--%d\n", getpid());
        sleep(5);
        exit(0);
    }else if (pid > 0) {
        //这段代码只有父进程才能运行到，
        printf("this is parent!!--%d\n", getpid());
    }
    //下边的代码，父子进程都有可能运行到
    while(1) {
        printf("------%d\n", getpid());
        sleep(1);
    }
    return 0;
}
