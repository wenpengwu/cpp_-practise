
#ifdef __cplusplus

extern "C"
{
#endif

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef __cplusplus
}
#endif

int main(){

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(sock, (struct sockaddr *) &sockAddr, sizeof(sockAddr));

    char buffer[] = "PING";
    send(sock,buffer,sizeof(buffer)-1,MSG_NOSIGNAL);
    printf("send %s\n", buffer);
    recv(sock, buffer, sizeof(buffer) - 1, MSG_NOSIGNAL);

    shutdown(sock, SHUT_RDWR);
    close(sock);

    printf("get %s\n", buffer);
    return 0;
}
