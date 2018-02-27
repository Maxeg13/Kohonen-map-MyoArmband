#include<stdint.h>
#ifndef STANDART_DEV
#define STANDART_DEV
#include <vector>
#include <iostream>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;

void getFeaturesMyo(vector<float>, vector<float>&);

void preproc(vector<float>&);

void getFeaturesKhor(vector<float>&, vector<float>&, int& ind);

void getFeatures_gearbox1(int8_t x, vector<float>&);

void getFeatures_gearbox2(int8_t x, vector<float>&);

class linearTr
{
public:
    linearTr( vector<float>,  vector<float>);
    linearTr(float, float, float, float);
    MatrixXd m;
    MatrixXd inp;
    MatrixXd out;
    vector<float> outv;
    MatrixXd inv_m;
    void setM();
    void inv();
    void proect(vector<float>&, int, int);


};















#endif
