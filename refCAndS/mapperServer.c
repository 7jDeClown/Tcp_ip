#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define ERR_EXIT(m) do{perror(m);exit(EXIT_FAILURE);}while(0)
int main(int argc, const char * argv[]) {
    int listenfd;
    if ((listenfd=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0) {  //也可将IPPROTO_TCP变成0，由前面两个参数已经确定这就是TCP协议
        ERR_EXIT("socket");
    }
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5188);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    /*  servaddr.sin_addr.s_addr=inet_addr("127.0.0.1);    */
    /*  inet_aton("127.0.01", &servaddr.sin_addr.);   */
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0) {
          ERR_EXIT("socket");
    }
    if (listen(listenfd, SOMAXCONN)) {
        ERR_EXIT("listen");
    }
    struct sockaddr_in peeraddr;
    socklen_t peerlen=sizeof(peeraddr);
    int conn;
    if ((conn=accept(listenfd,(struct sockaddr*)&peeraddr ,&peerlen))<0) {
        ERR_EXIT("accept");
    }
    char recvbuf[1024];
    while (1) {
        memset(recvbuf, 0, sizeof(recvbuf));
        int ret=read(conn, recvbuf, sizeof(recvbuf));
        fputs(recvbuf, stdout);
        write(conn, recvbuf, ret);
    }
    close(conn);
    close(listenfd);
    return 0;
}
