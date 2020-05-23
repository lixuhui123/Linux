#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>
int main()
{
  umask(0);
  int fd=open("./text.txt",O_RDWR|O_CREAT,0777);
  if(fd<0)
  {
    perror("open error");
    return -1;
  }
  char * ptr="天已经黑了\n";
 int ret= write(fd,ptr,strlen(ptr));

 if(ret<0)
 {
   perror("write error");
   return -1;
 }
 lseek(fd,0,SEEK_SET);
 char buf[1024]="";
 ret=read(fd,buf,sizeof(buf)-1);
 if(ret<0)
 {
   perror("read error");
   return -1;
 }
 else if(ret==0)
 {
   printf("at end of file\n");
 }
 printf("read buf :[%s]-[%d]\n",buf,ret);
 close(fd);
  return 0;
}

