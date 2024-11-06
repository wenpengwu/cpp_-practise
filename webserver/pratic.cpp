#include <iostream>
#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

const int kReadEv = 1;
const int kWriteEv=2;

void setNonBlock(int fd){
    int flags = fcntl(fd,F_GETFL,0);
    if(flags<0){
        cout<<"fcntl failed";
        exit(1);
    }
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if(r<0){
        cout<<"fcntl failed";
        exit(1);
    }

}

void updateEvents(int efd,int fd,int events,bool modify){
    struct kevent ev[2];
    int n=0;
    if(events&kReadEv){
        EV_SET(&ev[n++],fd,EVFILT_READ,EV_ADD|EV_ENABLE,0,0,(void*)(intptr_t)fd);
    }else if(modify){
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    }

    if (events & kWriteEv) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    } else if (modify){
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    }
    printf("%s fd %d events read %d write %d\n",
           modify ? "mod" : "add", fd, events & kReadEv, events & kWriteEv);

    if(kevent(efd,ev,n,NULL,0,NULL)){
        cout<<"kevent failed ";
        exit(1);
    }


}

void handleAccept(int efd,int fd){
    struct sockaddr_in raddr;
    socklen_t rsz = sizeof(raddr);
    int cfd = accept(fd,(struct sockaddr *)&raddr,&rsz);

    sockaddr_in peer, local;
    socklen_t alen = sizeof(peer);
    int r = getpeername(cfd, (sockaddr*)&peer, &alen);
    if(r<0){
        printf("getpeername failed");
        exit(1);
    }

    printf("accept a connection from %s\n", inet_ntoa(raddr.sin_addr));
    setNonBlock(cfd);
    updateEvents(efd, cfd, kReadEv|kWriteEv, false);

}

void handleRead(int efd,int fd){
    char buf[4096];
    int n = 0;
    while ((n=::read(fd, buf, sizeof buf)) > 0) {
        printf("read %d bytes\n", n);
        int r = ::write(fd, buf, n); //写出读取的数据
        //实际应用中，写出数据可能会返回EAGAIN，此时应当监听可写事件，当可写时再把数据写出

    }
    if (n<0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        return;
    printf("fd %d closed\n", fd);
    close(fd);

}

void handleWrite(int efd,int fd){
    //实际应用应当实现可写时写出数据，无数据可写才关闭可写事件
    updateEvents(efd, fd, kReadEv, true);

}

void loop_once(int efd,int lfd,int waitms){
    struct timespec timeout;
    timeout.tv_sec = waitms / 1000;
    timeout.tv_nsec = (waitms % 1000) * 1000 * 1000;
    const int kMaxEvents = 20;
    struct kevent activeEvs[kMaxEvents];
    //类似于 epoll_wait
    int n = kevent(efd,NULL,0,activeEvs,kMaxEvents,&timeout);
    printf("epoll_wait return %d\n", n);

    for(int i=0;i<n;i++){
        int fd = (int)(intptr_t)activeEvs[i].udata;
        int events = activeEvs[i].filter;
        if(events==EVFILT_READ){
            if(fd==lfd){
                handleAccept(efd, fd);
            }else{
                 handleRead(efd, fd);
            }

        }else if(events==EVFILT_WRITE){
            handleWrite(efd, fd);
        }else{
            cout<<"unkonwn event";
            exit(1);
        }
    }

}




int main(){
    int port = 90;
    int epollfd =kqueue();
    if(epollfd<0){
        cout<<"epoll_create failed";
        exit(1);
    }
    //创建socket
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0){
        cout<<"socket failed";
        exit(1);
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int r = ::bind(listenfd,(struct sockaddr *)&addr, sizeof(struct sockaddr));
    if(r){
        printf("bind to 0.0.0.0:%d failed %d %s",port,errno,strerror(errno));
        exit(1);
    }
    r = listen(listenfd,20);
    if(r){
        printf( "listen failed %d %s", errno, strerror(errno));
        exit(1);
    }
    printf("fd %d listening at %d\n", listenfd, port);
    updateEvents(epollfd,listenfd,kReadEv,false);

    for(;;){
        loop_once(epollfd,listenfd,10000);
    }


    cout<<"hello partic";
    return 0;
}
