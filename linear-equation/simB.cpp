#include <iostream>
#include <fncs.hpp>
#include <string>
#include <cmath>

using namespace std;
#ifndef nullptr 
#define nullptr NULL
#endif

static fncs::time time_current = 0;
static fncs::time time_stop = 5 * 60 * 3;
double a1 = 0.25;
double x_2_last = -1000;
static void generic_simulator()
{
    fncs::time time_granted=0;
    fncs::time time_desired=0;
    fncs::initialize();
    while (time_current < time_stop) {
        string x_1 = fncs::get_value("x_1");
        string f_2 = fncs::get_value("f_2");
        double x_1d = atof(x_1.c_str());
        double f_2d = atof(f_2.c_str());
        double x_2 = a1 * x_1d + f_2d;
        cout << "current " << time_current 
            << " f " << f_2 
            << " x_1 " << x_1 
            << " x_2 " << x_2 << endl;
       
        if (std::fabs(x_2_last - x_2) > 0.001 * std::fabs(x_2)) {
            fncs::publish("x_2", std::to_string(x_2));
            time_desired = time_current + 1;
            x_2_last = x_2;
        } else {
            time_desired = time_current + 5 * 60;
        }
        
        time_granted = fncs::time_request(time_desired);

        cout << "SimB: time_request"
            << " current=" << time_current
            << " desired=" << time_desired
            << " granted=" << time_granted << endl;

        time_current = time_granted;
    }
    cout << "SimB: DONE!" << endl;

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

