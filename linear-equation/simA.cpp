#include <iostream>
#include <fncs.hpp>
#include <string>
#include <cmath>

using namespace std;
#ifndef nullptr 
#define nullptr NULL
#endif

static fncs::time time_current = 0;
static fncs::time time_stop = 5 * 60 * 3 ;
double a2 = 0.25;
double x_1_last = -1000;
static void generic_simulator()
{
    fncs::time time_granted=0;
    fncs::time time_desired=0;
    fncs::initialize();

    while (time_current < time_stop) {
        string x_2 = fncs::get_value("x_2");
        string f_1 = fncs::get_value("f_1");
        double x_2d = atof(x_2.c_str());
        double f_1d = atof(f_1.c_str());
        double x_1 = a2 * x_2d + f_1d;
        cout << "current " << time_current 
            << " f " << f_1 
            << " x_2 " << x_2 
            << " x_1 " << x_1 << endl;

        if (std::fabs(x_1_last - x_1) > 0.001 * std::fabs(x_1)) {
            fncs::publish("x_1", std::to_string(x_1));
            time_desired = time_current + 1;
            x_1_last = x_1;
        } else {
            time_desired = time_current + 5 * 60;
        }

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

