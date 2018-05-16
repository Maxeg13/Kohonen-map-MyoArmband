#ifndef HISTOGRAMM_H
#define HISTOGRAMM_H
#include <vector>
using namespace std;

class Histogramm
{
public:
    int** b_ind;
    Histogramm();
    vector<float> b;
    int N;
    int N2;
    int width;
    int* grid;
    int* grid_out;
    float** a;
    float k;
    int range;
    int discr;
    float incr;

    void increment(int x, int y);

};

#endif // HISTOGRAMM_H
