//#include <iostream>
#include <vector>
//#include <Eigen/Eigenvalues>
//#include <Eigen/Dense>
//using namespace Eigen;
using namespace std;
//typedef std::complex<double> cd;


#include "pca.h"
int main(int argc, char **argv)
{
PCA myPCA(20,4);
std::vector<float> v;
v.resize(4);
for (int i=0; i<50; i++)
{

    for(int j=0;j<4;j++)
        v[j]=(rand()%20)/20.;

    myPCA.updateBuf(v);
}

myPCA.centr();
myPCA.getCor();
myPCA.algorithm();
myPCA.sort();
myPCA.proect(3,v);

cout<<v[1]<<endl;
//    MatrixXd MH = MatrixXd::Zero(8,3);
//    MatrixXd M=MH.transpose()*MH;
//    cout  << endl << MH << endl << endl;
//    cout  << endl << norm(MH) << endl << endl;
//     cout  << endl << norm(MH).mean();
// cout << endl <<  (M.middleCols<1>(1).max())<< endl << endl;
//eig();


//    Matrix3f m = Matrix3f::Random();
//    int i,j;
//     float minOfM = m.middleCols<1>(1).maxCoeff(&i,&j);
//     cout << "Here is the matrix m:\n" << m << endl;
//     cout << "Its minimum coefficient (" << minOfM
//          << ") is at position (" << i << "," << j << ")\n\n";


}
