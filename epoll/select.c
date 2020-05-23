/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：这个demo演示select的基本使用 
================================================================*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>

int main()
{
    //对标准输入进行监控
    fd_set rfds;//定义可读事件的描述符集合
    FD_ZERO(&rfds);//初始化集合
    while(1) {
        //将0号描述符添加到集合中，将位图的第0位置1,表示0号描述符需要监控
        //将标准输入添加到监控集合-select会修改集合内容,因此需要每次重新添加
        FD_SET(0, &rfds); 
        struct timeval tv;//定义select监控等待超时时间
        tv.tv_sec = 3;//3秒内若没有描述符就绪，则监控等待超时
        tv.tv_usec = 0;
        //select(最大描述符+1，可读事件集合，可写事件集合，异常事件集合，阻塞超时时间)
        ////将集合拷贝到内核中轮询遍历判断是否就绪
        int maxfd = 0;
        int nfds = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if (nfds == 0) {
            printf("监控超时...\n");
            continue;
        }else if (nfds < 0) {
            perror("监控出错");
            return -1;
        }
        //select返回时会从集合中移除所有未就绪的描述符
        if (FD_ISSET(0, &rfds)) {
            printf("标准输入事件就绪...进行读操作\n");
            char buf[1024] = {0};
            int ret = read(0, buf, 1023);
            if (ret <= 0) {
                perror("read error");
                return -1;
            }
            printf("read data from stdin:%s\n", buf);
        }
    }
    return 0;
}
