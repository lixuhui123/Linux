//#include<stdio.h>
//#include<unistd.h>
//#include<stdlib.h>
//#include<sys/shm.h>
//#define IPC_KEY 0x12345678//自己定义的共享内存的标识符
//#define PROJ_ID 0x12345678
//int main()
//{
//
//    int shmid=shmget(IPC_KEY,32,IPC_CREAT|0664);
//    if(shmid<0)
//    {
//        perror("shmget error");
//        return -1;
//    }
//    void* shmstart =shmat(shmid,NULL,0);
//    if(shmstart ==(void*)-1)
//    {
//        perror("shmat error");
//        return -1;
//    }
//    int i=0;
//    while(1)
//    {
//        
//        //覆盖式写入的方式，第二次写入的时候将前面的内容覆盖掉，首地址开始写的
//        printf("%s",shmstart);
//        sleep(1);
//    }
//    shmdt(shmstart);
//    shmctl(shmid,IPC_RMID,NULL);
//    return 0;
//}
