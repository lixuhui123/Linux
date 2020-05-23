/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：这个demo演示单例模式的实现 
================================================================*/

#include <cstdio>

/* 饿汉方式的单例模式实现
class singleton
{
    private: 
        //static将资源设置为静态成员-在程序初始化阶段完成资源的初始化加载
        //并且实例化的对象之间共用同一份资源
        static int _data; 
    public:
        int *get_instance() {
            return &_data;
        }
};
int singleton::_data = 10;
*/

#include <iostream>
#include <mutex>
std::mutex mutex;
class singleton 
{
    private:
        //初始化阶段只是初始化了指针并没有加载资源
        //但是这个指针是所有对象共用的
        volatile static int *_data; // 防止编译器过度优化，每次判断都为NULL，申请资源
    public:
        volatile int *get_instance(){
            //若资源已经加载成功,后续访问资源的线程依然会加锁判断是否为空,平白多了加锁解锁操作
            //对资源进行锁外二次判断，降低访问锁的概率提高效率
            if (_data == NULL) {
                mutex.lock();//考虑当前资源访问的线程安全问题
                if (_data == NULL) {//在访问这个资源的时候才会加载资源完成资源的初始化
                    _data = new int;//访问+申请资源的过程并非原子操作
                }
                mutex.unlock();
            }
            return _data;
        }
};
volatile int *singleton::_data = NULL;

int main()
{
    singleton a;
    singleton c;
    
    //单例模式就是资源只有一份--不管对象有多少个,资源是共有的只有一份
    printf("%p-%p\n", a.get_instance(), c.get_instance());
    return 0;
}
