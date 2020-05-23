//测试所有写端和读端被关闭的情况
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
int main()
{
    int pipefd[2]={0};
    int ret = pipe(pipefd);
    if(ret<0)
    {
        perror("pipe error");
        return -1;
    }
    int pid=fork();
    if(pid<0)
    {
        perror("fork error");
        return -1;
    }
    else if(pid==0)
    {
        //子进程会进入
       // sleep(1);
       // close(pipefd[1]);
        close(pipefd[0]);
       pause();
        char buf[1024]={0};
        int ret =read(pipefd[0],buf,1023);
        printf("buf:[%s]-[%d]\n",buf,ret);
    
    }
    else
    {

       // close(pipefd[1]);
       // pause();
       close(pipefd[0]);
       sleep(1);
       //父进程
        char *ptr="你是个憨憨\n";

        int ret= write(pipefd[1],ptr,strlen(ptr));
    }
    
    return 0;
}
