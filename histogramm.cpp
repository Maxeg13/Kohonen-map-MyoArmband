#include "histogramm.h"

Histogramm::Histogramm()
{
    N=7;
    k=0.999;
    width=10;
    a=new float*[N];
    for (int i=0;i<N;i++)
        a[i]=new float[N];


}
