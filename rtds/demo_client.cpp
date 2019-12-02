#include "tcp.hpp"

#include <iostream>
// 抓包 sudo tcpdump -i en0 '(tcp) and (host 10.10.8.208)' -v
using namespace std;

int main()
{
    unsigned char szBuff[BUFSIZ] = {0};
    float result;
    unsigned char tmp;
    unsigned char send[4];
    int nFd;
    int nReadLen = 0;
    
    nFd = connectToServer("10.10.8.208", 5843);
    if (nFd != -1) {
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
}
