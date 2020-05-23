//互斥锁的应用，黄牛抢票
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
int ticket =1000;
pthread_mutex_t mutex;
void *thr_tout(void * arg)
{
    
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(ticket>0)

        {
             usleep(5000);
          //sleep(1);
            printf("tout:%p - get a ticket :%d\n",pthread_self(),ticket);
            ticket--;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            //加锁之后再任意有可能退出线程的地方都要解锁
            pthread_mutex_unlock(&mutex);
            printf("tout:%p  exit \n",pthread_self());
            pthread_exit(NULL);
        }
    }
    return NULL;
}

int main()
{
    int i=0,ret;
    pthread_t tid[4];
    //互斥锁的初始化
    //pthread_mutex_init()
    //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIAIZER;
    pthread_mutex_init(&mutex,NULL);

    for(;i<4;++i)
    {
        ret=pthread_create(&tid[i],NULL,thr_tout,NULL);
        if(ret != 0)
        {
            printf("thread create error\n");
            return -1;
        }
    }
    for(i=0;i<4;++i)
    {
        pthread_join(tid[i],NULL);
    }
    //互斥锁初始化放在创建线程之前，销毁放在所有线程退出完毕之后
    pthread_mutex_destroy(&mutex); 
    return 0;
}
