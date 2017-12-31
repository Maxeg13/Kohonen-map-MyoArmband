#include<stdint.h>
#ifndef STANDART_DEV
#define STANDART_DEV
#include <vector>
#include <Eigen/Dense>
using Eigen::MatrixXd;

void getFeaturesMyo(std::vector<float>, std::vector<float>&);

void getFeatures_gearbox1(int8_t x, std::vector<float>&);

void getFeatures_gearbox2(int8_t x, std::vector<float>&);

class linearTr
{
public:
    linearTr( std::vector<float>,  std::vector<float>);
    MatrixXd m;
    MatrixXd inv;
    void setM();
    void inv();


};















#endif
