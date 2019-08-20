#include <iostream>
#include <fncs.hpp>
#include <string>
#include <cmath>

using namespace std;
#ifndef nullptr 
#define nullptr NULL
#endif
int deltaT = 5 * 60;
static fncs::time time_current = 0;
static fncs::time time_stop = deltaT * 4;

static void generic_simulator()
{
    fncs::time time_granted=0;
    fncs::time time_desired=0; 
    fncs::initialize();
    while (time_current < time_stop) {
        string x_2 = fncs::get_value("x_2");
        string x_1 = fncs::get_value("x_1");

        cout << "current " << time_current << " x_1 " << x_1 << " x_2 " << x_2 << endl;
        fncs::publish("f_1", std::to_string(time_current));
        fncs::publish("f_2", std::to_string(time_current));
        time_desired = time_current + deltaT;

        time_granted = fncs::time_request(time_desired);

        cout << "SimA: time_request"
            << " current=" << time_current
            << " desired=" << time_desired
            << " granted=" << time_granted << endl;

        time_current = time_granted;
    }
    cout << "SimA: DONE!" << endl;

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

