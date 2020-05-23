#include <iostream>
#include <string>
#include <vector>
#include "epoll.hpp"

int main(int argc, char *argv[]) 
{
    if (argc != 3) {
        printf("usage: ./epoll_srv ip port\n");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[2]);

    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket());
    //将描述符设置为非阻塞
    lst_sock.SetNonBlock();
    CHECK_RET(lst_sock.Bind(ip, port));
    CHECK_RET(lst_sock.Listen());

    Epoll epoll;
    CHECK_RET(epoll.Add(lst_sock));

    while(1) {
        std::vector<TcpSocket> list;
        bool ret = epoll.Wait(&list);
        if (ret == false) {
            return -1;
        }
        for (int i = 0; i < list.size(); i++) {
            if (list[i].GetFd() == lst_sock.GetFd()) {
                TcpSocket cli_sock;
                bool ret = lst_sock.Accept(&cli_sock);
                if (ret == false) {
                    continue;
                }
                cli_sock.SetNonBlock();
                epoll.Add(cli_sock);
            }else {
                std::string buf;
                ret = list[i].Recv(&buf);
                if (ret == false) {
                    epoll.Del(list[i]);
                    list[i].Close();
                    continue;
                }
                std::cout << "client say: " << buf << std::endl;

                std::cout << "server say: ";
                buf.clear();
                std::cin >> buf;
                ret = list[i].Send(buf);
                if (ret == false) {
                    epoll.Del(list[i]);
                    list[i].Close();
                }
            }
        }
    }
    lst_sock.Close();
    return 0;
}
