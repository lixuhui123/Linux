#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
int main()
{
    close(1);
    umask(0);
    int fd=open("./test.txt",O_RDWR | O_CREAT ,0777);
    if(fd<0)
    {
        perror("open error");
        return -1;
    }
    printf("fd:%d\n",fd);
fflush(stdout);
    close(fd);
    /*
   close(1);
    printf("printf");
    fwrite("fwrite",6,1,stdout);
    fprintf(stdout,"%s","fprintf");
    write(1,"write",5);
    sleep(3);*/
    return 0;
}
