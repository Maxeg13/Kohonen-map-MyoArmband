
#include <iostream>
#include "pca.h"
#include <vector>
#include <Eigen/Dense>
#include "eig.h"
#include <math.h>



void PCA::algorithm()
{
    eig(cor);
    eigvals=getEigVals();
    eigvecs=getEigVecs();
	inverseEigens=eigvecs.inverse();
    cout<<endl<<"eigvecs:"<<endl<<eigvecs<<endl;
    cout<<endl<<"eigenvals"<<endl<<eigvals<<endl;
	cout<<endl<<"inverseEinens"<<endl<<inverseEigens<<endl;
}

void PCA::proect(std::vector<float>&v)
{

//cout<<endl<<"features:"<<features<<endl;
    featuresOut=inverseEigens*features.transpose();
    for(int i=0;i<v.size();i++)
        v[i]=featuresOut(i,0)/sqrt(eigvals(i,0))*2000;
//cout<<endl<<"proect2"<<endl;

}

void PCA::sort()
{
    //        MatrixXd mo=MatrixXd::Random(width,width);
    //        MatrixXd v=MatrixXd::Random(width,1)+MatrixXd::Constant(width,1,1);
    //        v=v.abs();
    //        cout<<v<<endl;
    eigvecs_s=eigvecs;
    int* a=new int[width];
    for(int i=0;i<width;i++)
    {
        int j=0;int k=0;
        eigvals.maxCoeff(&j,&k);
        eigvals(j)=0;
        a[i]=j;
        //            cout<<a[i]<<" ";

        eigvecs_s.col(i)=eigvecs.col(j);

    }
    //        cout<<endl<<m<<endl<<endl;
    //        cout<<endl<<mo;
    cout<<endl<<"eigvecs after:"<<endl<<eigvecs_s<<endl;
    delete a;
}


void PCA::getCor()
{

    cor=buf.transpose()*buf;
    cout<<"correlation matr:"<<endl<< cor<<endl;
}

void PCA::updateBuf(std::vector<float> x)
{
    for(int i=0;i<width;i++)
        buf(ind,i)=x[i];

    features=buf.row(ind);

    ind++;
    if(ind==size) ind=0;

    //cout<<"features:"<<endl<< features<<endl;
}

PCA::PCA(int x,int y)
{
    buf=MatrixXd::Zero(x,y);
    //        buf=MatrixXd::Random(30,4);
    size=x;
    width=y;
	eigvecs_s=MatrixXd::Identity(width,width);
    //        cout<<buf;
}


void PCA::centr()
{
    MatrixXd v=MatrixXd::Ones(buf.rows(),1);
    buf_c=buf;
    for(int i=0;i<buf.cols();i++)
    {
        buf_c.col(i)-=buf.col(i).mean()*v;
//		buf_c.col(i)/=buf_c.col(i).maxCoeff();
    }
}




