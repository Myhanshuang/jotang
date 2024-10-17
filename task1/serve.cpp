#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h> // sockaddr_in in here
#include <unistd.h>     //close here
const int N = 4096;
int main(int argv, char *argc[])
{
    int listenfd, connectfd; // 用于监听的 socket 和 用于连接的 socket
    sockaddr_in servaddr;    // 服务端的地址和端口
    char buf[N];
    int n;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        perror("socket create fail");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof servaddr);
    servaddr.sin_family = AF_INET; // IPV4
    // INADDR_ANY = 0.0.0.0
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 转换本机字节序为网络字节序
    servaddr.sin_port = htons(8888);              // 转换端口号
    // 绑定的socket id to servaddr
    if (bind(listenfd, (sockaddr *)&servaddr, sizeof servaddr))
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }
    if (listen(listenfd, 10) == -1)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    puts("waiting for the client");
    while (1)
    {
        connectfd = accept(listenfd, nullptr, nullptr);
        // 尝试进行连接
        if (connectfd == -1)
            continue;
        n = recv(connectfd, buf, N, 0);
        buf[n] = '\0';
        send(connectfd, buf, N, 0);
        close(connectfd);
    }
    close(listenfd);
}