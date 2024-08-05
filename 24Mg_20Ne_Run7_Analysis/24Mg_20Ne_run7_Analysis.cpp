#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>

#define PI 3.14159
using namespace std;

int main() {

    int event_num;
    string filename;
    cout << "Input file name (including extension) and event number: " << endl;
    cin >> filename >> event_num;

    ofstream datafile("24Mg_20Ne_Run7_Slopes_Intercepts.txt", ios::app);
    //datafile << "Event:" << "  " << "Slope:" << "  " << "Intercepts:" << endl;

    ifstream myfile(filename);
    if (!myfile) {cerr << "This file does not exist!" << endl;}
    
    vector<float> x_values, y_values, charge;
    vector<int> pad_num, time_bin;
    int pad, time;
    float x_coord, y_coord, q;
    
    while (myfile >> x_coord >> y_coord >> time >> pad >> q) {

        x_values.push_back(x_coord);
        y_values.push_back(y_coord);
        time_bin.push_back(time);
        pad_num.push_back(pad);
        charge.push_back(q);
    }

    int theta_high, theta_low, increment, bins, difference;
    cout << "Input lowest and highest theta values and increment:" << endl;
    cin >> theta_low >> theta_high >> increment;

    difference = theta_high - theta_low;
    bins = difference/increment + 1;

    int rmax = 260 * 2;
    int accumulator[rmax][bins] = {0};
    float radians, r_float;
    int rounded_rfloat;

    for (int i = 0; i < x_values.size(); i++) {
        for (int j = theta_low; j <= theta_high; j+=increment) {
            
                radians = (float)j * PI/180.0;
                r_float = x_values[i]*cos(radians) + y_values[i]*sin(radians);
                rounded_rfloat = round(r_float);
                accumulator[rounded_rfloat][(j-theta_low)/increment] += 1; 
        }
    }

    int biggest = accumulator[0][0];
    float slopes, intercepts, rad, radial_values;

    int r_int1;
    float r_float1, radians1; 
    float cut_rmax, cut_rmin, cut_theta_max, cut_theta_min; // Maybe user inputs these values?

    cout << "Define regions of the \'cut\' with rmax, rmin, theta max, and theta min (all floats)" << endl;
    cin >> cut_rmax >> cut_rmin >> cut_theta_max >> cut_theta_min;
    
    for (int i = 0; i < rmax; i++) {
        int angle = 0;
        
        for (int j = 0; j < bins; j++) {
            angle = theta_low + j*increment;
            
            if (accumulator[i][j] > 2 && ((float)i <= cut_rmax && (float)i >= cut_rmin)
            && ((float)j <= cut_theta_max && (float)j >= cut_theta_min)) {
                
                biggest = accumulator[i][j];
                rad = (float)angle * PI/180.0;
                radial_values = (float)i;

                slopes = -cos(rad)/sin(rad);
                intercepts = radial_values/sin(rad);

                for (int k = 0; k < x_values.size(); k++) {

                    r_float1 = x_values[k]*cos(rad) + y_values[k]*sin(rad);
                    r_int1 = (int)r_float1;
                    if (r_int1 <= (float)i + 0.5 && r_int1 >= (float)i - 0.5) {

                        cout << "X Values: " << x_values[k] << " Y Values: " << y_values[k] <<
                        " Number of Intercepts: " << biggest << endl;
                    }
                }
            }
        }
    }

    datafile << event_num << "  " << slopes << "  " << intercepts << endl;  
    return 0;
}