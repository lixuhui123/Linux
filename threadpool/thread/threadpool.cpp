/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：这个demo实现一个简单的线程池类以及任务类 
================================================================*/

#include <cstdio>
#include <queue>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef void(*task_handler_t)(int);
class ThreadTask
{
    public:
        ThreadTask(int data, task_handler_t handler):
            _data(data), _handler(handler){
        }
        void Run() {
            return _handler(_data);//用传入的方法处理传入的数据
        }
    private:
        int _data;//这是用户传入的要处理的数据
        task_handler_t _handler;//这是用户传入的数据的处理方法
};

#define MAX_THREAD 5
class ThreadPool
{
    public:
        ThreadPool(int max_thread = MAX_THREAD):_thr_count(max_thread){
            pthread_mutex_init(&_mutex, NULL);
            pthread_cond_init(&_cond, NULL);
            //sem_init(&sem, 0, 0);
            for (int i = 0; i < _thr_count; i++) {
                pthread_t tid;
                int ret = pthread_create(&tid, NULL, thr_start, (void*)this);
                if (ret != 0) {
                    printf("create thread error\n");
                    exit(0);
                }
                //若对线程的返回值并不关心，并且希望线程退出后能够自动释放资源
                pthread_detach(tid); // 分离这个线程
            }
        }
        ~ThreadPool(){
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_cond);
            //sem_destroy(&sem);
        }
        bool PushTask(const ThreadTask &task){
            //像线程池外部提供的任务入队操作
            //能够入队的其实都是生产者,因为没有做队列的最大节点限制，因此生产者不需要阻塞
            //只需要保护task_queue的操作就可以
            pthread_mutex_lock(&_mutex);
            _task_queue.push(task);
            pthread_mutex_unlock(&_mutex);
            pthread_cond_signal(&_cond);//唤醒线程池中的线程处理任务
            //sem_post(&sem)
            return true;
        }
    private:
        //这是线程池中线程的入口函数
        static void *thr_start(void *arg) {
            //获取一个任务，然后调用任务对象的Run接口
            //线程池中的线程并不会处理一个任务之后就退出，因此是一个死循环
            ThreadPool *pool = (ThreadPool*)arg;
            while(1) {
                //判断队列是否位为NULL，则线程需要阻塞等待
                pthread_mutex_lock(&pool->_mutex);
                while (pool->_task_queue.empty()) {
                    pthread_cond_wait(&pool->_cond, &pool->_mutex);
                }
                //sem_wait()
                //pthread_mutex_lock()
                ThreadTask task = pool->_task_queue.front();//获取队首节点
                pool->_task_queue.pop();//队首节点出队操作
                pthread_mutex_unlock(&pool->_mutex);

                //解锁之后再进行任务处理，否则，会造成当前线程加锁获取任务进行处理期间
                //其它线程无法获取锁，导致无法处理任务---演变成为任务的串行化处理
                //并且加锁是为了保护task_queue的操作，而不是为了保护任务处理过程的
                task.Run();// 使用任务中用户传入的处理函数处理传入的数据
            }
            return NULL;
        }
    private:
        int _thr_count;//线程池中线程的数量
        std::queue<ThreadTask> _task_queue;
        pthread_mutex_t _mutex;//互斥保护_task_queue的操作
        pthread_cond_t _cond;//线程池中线程等待的队列
};


void test(int data)//用户自己定义的数据处理函数
{
    srand(time(NULL));
    int sec = rand() % 5;
    printf("thread:%p  get data:%d  sleep %d sec\n", pthread_self(), data, sec);
    sleep(sec);
    return;
}
int main()
{
    ThreadPool pool;

    for (int i = 0; i < 10; i++) {
        ThreadTask task(i, test);
        pool.PushTask(task);
    }
    while(1) {
        sleep(1);
    }
    return 0;
}
