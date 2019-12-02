#include <fncs.hpp>
#include "tcp.hpp"
#include <string>
#include <iostream>

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
    fncs::time time_granted=0; 
    fncs::time time_desired=0; 
    fncs::initialize();

    int nFd = 0;
    int nReadLen = 0;
    unsigned char szBuff[BUFSIZ] = {0};
    float result;
    unsigned char tmp;
    unsigned char send[4];

    nFd = connectToServer("10.10.8.209", 5843);

    if (nFd != -1) {
        while(1)
        {
            memset(szBuff,0,BUFSIZ);
            memset(send,0, sizeof(send));
            nReadLen = read(nFd,szBuff,BUFSIZ);
            if (nReadLen > 0) {
                result = fourBytesToFloat(szBuff);
                fncs::publish("published", std::to_string(result));
                time_desired = time_current + 1;
                time_granted = fncs::time_request(time_desired);
                cout << "SimA: time_request"
                    << " current=" << time_current
                    << " desired=" << time_desired
                    << " granted=" << time_granted << endl;

                string received = fncs::get_value("received");
                floatToFourBytes(atof(received.c_str()), send);
                write(nFd,send,sizeof(send));
            }
            time_current = time_granted;
        }
    }

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

