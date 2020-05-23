/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：封装epoll的基本操作；向外提供添加监控/移除监控/获取就绪信息操作 
================================================================*/

#include <cstdio>
#include <iostream>
#include <vector>
#include <sys/epoll.h>
#include "tcpsocket.hpp"

class Epoll
{
    public:
        Epoll(): _epfd(-1){
            //epoll_create(int maxevent) // maxevent已经忽略了，非0就可以
            _epfd = epoll_create(1);
            if (_epfd < 0) {
                perror("epoll_create error");
                exit(-1);//因为构造函数没有返回值,不知道构造成功与否,因此失败直接退出
            }
        }
        bool Add(TcpSocket &sock){
            int fd = sock.GetFd();
            //epoll_ctl(epoll句柄，操作， 描述符， 对应的事件结构)
            struct epoll_event ev;
            ev.events = EPOLLIN | EPOLLET;//EPOLLIN-可读事件
            ev.data.fd = fd;
            int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
            if (ret < 0) {
                perror("epoll_ctl add error");
                return false;
            }
            return true;
        }
        bool Del(TcpSocket &sock) { 
            int fd = sock.GetFd();
            int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
            if (ret < 0) {
                perror("epoll_ctl del error");
                return false;
            }
            return true;
        }
        bool Wait(std::vector<TcpSocket> *list, int timeout = 3000){
            //epoll_wait(操作句柄，事件数组首地址，数组节点数量，超时时间)
            struct epoll_event evs[10];
            int ret = epoll_wait(_epfd, evs, 10, timeout);
            if (ret < 0) {
                perror("epoll wait error");
                return false;
            }else if (ret == 0) {
                printf("timeout\n");
                return true;
            }
            //就绪的事件都放在evs中，就绪的个数就是返回值
            for (int i = 0; i < ret; i++) {
                if (evs[i].events & EPOLLIN) {//判断就绪的事件是否是可读事件
                    //这里其实可以不用判断，因为我们的添加函数就只监控了可读事件
                    TcpSocket sock;
                    sock.SetFd(evs[i].data.fd);
                    list->push_back(sock);
                }
            }
            return true;
        }
    private:
        int _epfd;//epoll的操作句柄
};
