/*
实现tcp的服务器端的功能
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

/* FNCS headers */
#include <fncs.hpp>
#include <string>

using namespace std; /* C++ standard namespace */

/* some C++ compilers have a nullptr instance,
 * otherwise we fall back to the old default of NULL */
#ifndef nullptr 
#define nullptr NULL
#endif

/* the current time of our simulator */
static fncs::time time_current=0;
/* time when sim will stop */
static fncs::time time_stop=1000;

/* our simulator function, called by the main() function later */
static int generic_simulator()
{
    fncs::time time_granted=0; /* the time step FNCS has allowed us to process */
    fncs::time time_desired=0; /* the time step we would like to go to next */

    /* initialize FNCS with default configuration file fncs.zpl */
    fncs::initialize();



    struct sockaddr_in serveraAddr , clientAddr;
    socklen_t clientAddrLen;
    int nFd = 0,linkFd = 0;
    int nRet = 0;
    int nReadLen = 0;
    char szBuff[BUFSIZ] = {0};

    /* 创建一个socket描述符 */
    nFd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == nFd)
    {
        perror("socket:");
        return -1;
    }

    /* 给本地的socket地址赋值 */
    memset(&serveraAddr,0,sizeof(struct sockaddr_in));
    serveraAddr.sin_family = AF_INET; //以ipv4协议进行连接
    serveraAddr.sin_addr.s_addr = htonl(INADDR_ANY); //接收所有客户端ip的连接
    serveraAddr.sin_port = htons(8080);//接收8080端口发来的连接

    /* 当TCP的连接的状态是TCP_WAIT状态的时候， 可以通过设置SO_REUSEADDR
        选项来强制使用属于TIME_WAIT状态的连接的socket*/
    int isReuse = 1;
    setsockopt(nFd,SOL_SOCKET,SO_REUSEADDR,(const char*)&isReuse,sizeof(isReuse));

    /* 将该socket的描述符和本地的套接字地址绑定起来 */
    nRet = bind(nFd,(struct sockaddr*)&serveraAddr,sizeof(serveraAddr));
    if(-1 == nRet)
    {
        perror("bind:");
        return -1;
    }

    /* 设置该套接口在监听状态 */
    listen(nFd,1);

    /* 等待客户端发来的tcp连接 ,当客户端连接进来之后，返回两个之间的唯一的socket连接，存放在linkFd之中*/
    clientAddrLen = sizeof(struct sockaddr_in);
    linkFd = accept(nFd,(struct sockaddr*)&clientAddr,&clientAddrLen);
    if(-1 == linkFd)
    {
        perror("accept:");
        return -1;
    }

    /* 把连接进来的客户端地址和端口打印出来 */
    printf("commect %s %d successful\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));

    while (time_current < time_stop) {
         memset(szBuff,0,BUFSIZ);
        nReadLen = read(linkFd,szBuff,BUFSIZ);
        if(nReadLen > 0)
        {
            cout << "Server: working" << szBuff <<endl;
            fncs::publish("msg", szBuff);
            
            time_desired = time_current + 1;
            time_granted = fncs::time_request(time_desired);
            cout << "server: time_request"
                << " current=" << time_current
                << " desired=" << time_desired
                << " granted=" << time_granted << endl;
            time_current = time_granted;

        }
    }
    cout << "Server: DONE!" << endl;
    fncs::finalize();
}

int main(int argc, char **argv)
{
    try {
        cout << "starting generic simulator" << endl;
        generic_simulator();
    } catch (const exception &e) {
        cout << e.what() << endl;
    }
    catch (const string &e) {
        cout << e << endl;
    }
    cout << "finished generic simulator" << endl;
    return 0;
}