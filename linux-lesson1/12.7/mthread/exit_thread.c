ps aux -L | grep exit_phreaduu(void * arg)
{
    //char buf[1023]="你吃早饭了吗";
    char * buf="我不吃早饭";
   // pthread_detach(pthread_self());
    sleep(1);
    pthread_exit(buf);
    while(1)
    {

        printf("主线程传递了参数：%s\n",(char*)arg);
        sleep(1);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    char buf[]="hahahhahahah\n";
    int ret=pthread_create(&tid,NULL,thread_start,(void *)buf);
    if(ret !=0)
    {
        printf("thread create error:%d\n",ret);
        return -1;
    }
    pthread_detach(tid);
    //sleep(3);
    //pthread_cancel(tid);
   // pthread_exit(NULL);
   
    //thread_cancel(pthread_self());
    //void **retval=(void **)malloc(1024);
    /*
    void **retval;
    pthread_join(tid,retval);
    printf("%s\n",*retval);
    */
    while(1)
    {
        printf("i am main thread:%p\n",tid);
       sleep(1);
    }
    return 0;
}
