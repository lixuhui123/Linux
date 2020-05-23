#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void sigcb(int signo)
{
    //若没有子进程退出，则waitpid返回0；
    while(waitpid(-1, NULL, WNOHANG) > 0);
}
int main()
{
    signal(SIGCHLD, sigcb);
    pid_t pid = fork();
    if (pid == 0) {
        sleep(5);
        exit(0);
    }
    if (fork() == 0){
        while(1) {
            printf("i am child\n");
            sleep(1);
        }
    }//再创建一个子进程，并不退出
    while(1) {
        printf("打一桌麻将\n");
        sleep(1);
    }
    return 0;
}
