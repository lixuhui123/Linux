/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：封装实现一个Select类，外部通过实例化的select对象实现
*             简单的并发服务器
*             class Selete{
*               添加描述符的监控
*               移除描述符的监控
*               开始监控，并且返回就绪的描述符
*               private:
*                 _maxfd;  // 集合中最大的描述符
*                 fd_set rfds;  用户添加的描述符都在这里边
*             }
================================================================*/

#include <iostream>
#include <vector>
#include <sys/select.h>
#include <time.h>
#include "tcpsocket.hpp"

class Select
{
    public:
        Select():_maxfd(-1){
            FD_ZERO(&_rfds);//初始化清空集合
        }
        bool Add(TcpSocket &sock) {
            //将描述符添加到监控集合
            int fd = sock.GetFd();
            FD_SET(fd, &_rfds);// 将描述符添加到集合中
            _maxfd = _maxfd > fd ? _maxfd : fd;//每次添加监控重新判断最大描述符
            return true;
        }
        bool Del(TcpSocket &sock) {
            //从监控集合中移除描述符
            int fd = sock.GetFd();
            FD_CLR(fd, &_rfds);// 从集合中移除描述符 // 0 3 5 8
            if (fd != _maxfd) {//判断删除的描述符是否是最大的描述符
                return true;
            }
            for (int i = fd; i >= 0; i--) {//重新判断最大的描述符是多少
                if (FD_ISSET(i, &_rfds)) {
                    _maxfd = i;
                    break;
                }
            }
            return true;
        }
        bool Wait(std::vector<TcpSocket> *list, int mtimeout = 3000) {
            //开始监控，并返回就绪的socket信息
            struct timeval tv;
            tv.tv_usec = (mtimeout % 1000) * 1000;
            tv.tv_sec = mtimeout / 1000;//mtimeout 单位为毫秒
            //因为select会修改描述符集合，返回时将未就绪的描述符全部移除
            //因此不能直接使用_rfds,而是使用临时集合，避免对_rfds做出修改
            fd_set tmp_rfds = _rfds;
            int nfds = select(_maxfd + 1, &tmp_rfds, NULL, NULL, &tv);
            if (nfds < 0) {//监控出错
                perror("select error");
                return false;
            }else if (nfds == 0) {//监控等待超时
                printf("wait timeout\n");
                return true;
            }
            for (int i = 0; i <= _maxfd; i++) {//这是一种笨办法，从0到_maxfd逐个判断谁在集合中
                if (FD_ISSET(i, &tmp_rfds)) {     //谁在集合，谁就是就绪的
                    //就绪的描述符
                    TcpSocket sock;
                    sock.SetFd(i);
                    list->push_back(sock);
                }
            }
            return true;
        }
    private:
        int _maxfd;  //最大的描述符
        fd_set _rfds;//可读事件的描述符集合
};
