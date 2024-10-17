#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
const int N = 4096;
int main(int argv, char *argc[])
{
    int connectfd;
    char buf[N];
    int n;
    connectfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connectfd == -1)
    {
        perror("socket fail");
        exit(EXIT_FAILURE);
    }
    sockaddr_in serve;
    serve.sin_family = AF_INET;
    serve.sin_port = htons(8888);
    if (inet_pton(AF_INET, argc[1], &serve.sin_addr) == 0)
    {
        perror("transform fail");
        exit(EXIT_FAILURE);
    }
    // connect fail
    //  puts("faksdhfoansdfa");

    if (connect(connectfd, (sockaddr *)&serve, sizeof serve) < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }

    // puts("faksdhfoansdfa");
    while (1)
    {
        memset(buf, 0, sizeof buf);
        fgets(buf, N, stdin);
        if (send(connectfd, buf, sizeof buf, 0) == -1)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }
        memset(buf, 0, sizeof buf);
        n = recv(connectfd, buf, N, 0);
        cout << buf << endl;
        close(connectfd);
        break;
    }
}