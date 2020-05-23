#include <cstdio>
#include "select.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("usage: ./select_srv ip port\n");
        return -1;
    }
    std::string srv_ip = argv[1];
    uint16_t srv_port = std::stoi(argv[2]);

    Select s;

    TcpSocket listen_sock;
    CHECK_RET(listen_sock.Socket());//创建套接字
    CHECK_RET(listen_sock.Bind(srv_ip, srv_port));//为套接字绑定地址信息
    CHECK_RET(listen_sock.Listen());//开始监听

    s.Add(listen_sock);
    while(1) {
        std::vector<TcpSocket> list;//
        bool ret = s.Wait(&list);
        if (ret == false) {
            return -1;
        }
        for (auto sock : list) {
            if (sock.GetFd() == listen_sock.GetFd()) {
                //这个就绪的套接字是监听套接字--表示由新连接到来
                TcpSocket client_sock;
                bool ret = listen_sock.Accept(&client_sock);//获取新建连接
                if (ret == false) {
                    continue;
                }
                s.Add(client_sock); //将通信套接字也添加监控，添加到永久集合中去。
            }else {
                //这个就绪的套接字是一个普通的通信套接字
                std::string buf;
                ret = sock.Recv(&buf);//使用新建连接接收客户端数据
                if (ret == false) {
                    s.Del(sock);//描述符出错则移除监控，永远不再监控了，可能是断开了连接
                    sock.Close();
                    continue;
                }
                std::cout << "client say: " << buf << std::endl;
                buf.clear();
                std::cout << "server say: ";
                std::cin >> buf;
                ret = sock.Send(buf);//通过新建连接向客户端发送数据
                if (ret == false) {
                    s.Del(sock);//描述符出错则移除监控
                    sock.Close();
                    continue;
                }
            }
        }
    }
    listen_sock.Close();
    return 0;
}
