/*
实现tcp的客户端的功能
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

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

    struct sockaddr_in serverAddr;
    int nFd = 0;
    int nRet = 0;
    int nReadLen = 0;
    char szBuff[BUFSIZ] = {0};

    /* 创建套接字描述符 */
    nFd = socket(AF_INET,SOCK_STREAM,0);
    if (-1 == nFd)
    {
        perror("socket:");
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("10.10.8.208");
    serverAddr.sin_port = htons(8080);//默认以8080端口连接

    /* 和服务器端建立连接 */
    nRet = connect(nFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if (nRet == -1)
    {
        printf("connect:");
        return -1;
    }
    /* unless FNCS detects another simulator terminates early, the
     * this simulator will run from time step 0 to time step 9 */
    while (time_current < time_stop) {
        /* do useful work; check for incoming messages to the objects we
         * registered earlier */
        cout << "Client: Working." << endl;
       
        /* Check for incoming messages. */
        string msg = fncs::get_value("msg");
        // strcpy(szBuff, msg.c_str());
        // 从终端读取数据
        szbuff = (char *)&msg;
        memset(szBuff,0,BUFSIZ);
        nReadLen = read(STDIN_FILENO,szBuff,BUFSIZ);
        if (nReadLen > 0)
        {
            write(nFd,szBuff,strlen(szBuff));
        }

        /* Which time step do we wish to go to next? This does not
         * necessarily need to be a delta of 1; FNCS supports
         * arbitrary time deltas. */
        time_desired = time_current + 1;

        /* Synchronization by requesting the next time step. The next
         * time could very well be smaller than this simulator is able
         * to process i.e. the network simulator runs in nanoseconds
         * while this simulator runs in seconds. In such a case, the
         * time_granted will be the same as time_current. Another
         * possibility is that the time_granted is less than
         * time_desired due to another simulator requesting a smaller
         * time delta. */
        time_granted = fncs::time_request(time_desired);

        cout << "Client: time_request"
            << " current=" << time_current
            << " desired=" << time_desired
            << " granted=" << time_granted << endl;

        time_current = time_granted;
    }
    cout << "Client: DONE!" << endl;

    fncs::finalize();
}


/* a simple main that calls our simulator function;
 * it is sometimes useful to capture and report exceptions but this
 * might be overkill for your particular needs */
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

