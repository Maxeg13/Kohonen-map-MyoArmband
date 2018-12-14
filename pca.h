#ifndef PCA_H
#define PCA_H
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "eig.h"
using namespace Eigen;
using namespace std;


class PCA
{
public:
    int ind;
    int size,width;
    void algorithm();
    void proect( std::vector<float>&);
    void sort();
    void getCor();
    void updateBuf(std::vector<float>);
    PCA(int ,int );
    void centr();
private:
    MatrixXd buf,buf_c;
    MatrixXd cor;
    MatrixXd eigvecs, eigvecs_s;
    MatrixXd eigvals;
	MatrixXd inverseEigens;
    MatrixXd features, featuresOut;
};


#endif // EIGEN_H
