#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#include <iostream>

using namespace std;

int fourBytesToInt(unsigned char buffer[4]) {
    int result = (buffer[0] & 0xFF) |
     ((buffer[1] << 8) & 0xFF00) |
     ((buffer[2] << 16 ) & 0xFF0000) |
     ((buffer[3] << 24) & 0xFF000000);
     return result;
}

float fourBytesToFloat(unsigned char buffer[4]) {
    float result;
    unsigned char swapBytesBuff[4];
    swapBytesBuff[0] = buffer[3];
    swapBytesBuff[1] = buffer[2];
    swapBytesBuff[2] = buffer[1];
    swapBytesBuff[3] = buffer[0];
    result = *(float *)(swapBytesBuff);
    return result;
}

void floatToFourBytes(float num, unsigned char buffer[4]) {
    unsigned char* tmp;
    tmp = (unsigned char*)(&num);
    buffer[0] = tmp[3];
    buffer[1] = tmp[2];
    buffer[2] = tmp[1];
    buffer[3] = tmp[0];
}

int main()
{
    struct sockaddr_in serverAddr;
    int nFd = 0;
    int nRet = 0;
    int nReadLen = 0;
    unsigned char szBuff[BUFSIZ] = {0};
    float result;
    unsigned char tmp;
    unsigned char send[4];

    /* 创建套接字描述符 */
    nFd = socket(AF_INET,SOCK_STREAM,0);
    if (-1 == nFd)
    {
        perror("socket:");
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("10.10.8.208");
    serverAddr.sin_port = htons(5843);

    /* 和服务器端建立连接 */
    nRet = connect(nFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if (nRet == -1)
    {
        printf("connect:");
        return -1;
    }
    while(1)
    {
        memset(szBuff,0,BUFSIZ);
        memset(send,0, sizeof(send));
        nReadLen = read(nFd,szBuff,BUFSIZ);
        
        if (nReadLen > 0) {
            result = fourBytesToFloat(szBuff);
            floatToFourBytes(result, send);
            write(nFd,send,sizeof(send));
        }
    }
}
