//这个demo可以认识到匿名管道的基本操作，并且体会到管道的各种特性
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
       pause();
        char buf[1024]={0};
        int ret =read(pipefd[0],buf,1023);
        printf("buf:[%s]-[%d]\n",buf,ret);
    
    }
    else
    {

        //父进程
       // sleep(1);
       int total=0;
        char *ptr="你是个憨憨\n";
        while(1)
        {

         int ret= write(pipefd[1],ptr,strlen(ptr));
        total+=ret;
        printf("total:%d\n",total);
        }
    }
   /* while(1)
    {
        printf("--------------%d\n",getpid());
        sleep(1);
    }*/
    return 0;
}
