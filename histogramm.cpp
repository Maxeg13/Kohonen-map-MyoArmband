#include "histogramm.h"

Histogramm::Histogramm()
{
    N=9;
    N2=N*N;
    k=0.996;
    width=20;
    range=150;
    discr=range/N;
    incr=3;

    b_ind=new int*[N];
    for(int i=0;i<N;i++)
        b_ind[i]=new int[N];

    grid_out=new int [N+1];

    grid=new int [N];

    for(int i=0;i<N;i++)
        grid[i]=-range/2+discr*i;

    for(int i=0;i<N;i++)
        grid_out[i]=i*width;

    a=new float*[N];
    for (int i=0;i<N;i++)
        a[i]=new float[N];

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            a[i][j]=0;

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            b_ind[i][j]=j+i*N;

    b.resize(N*N);
}

void Histogramm::increment(int x, int y)
{
    int ind=0;
    {
        for(int i=0;i<(N-1);i++)
            for(int j=0;j<(N-1);j++)
            {
                ind++;
                a[i][j]*=k;
                if((x>grid[j])&(x<grid[j+1]))
                    if((y>grid[i])&(y<grid[i+1]))
                    {
                        a[i][j]+=incr;
                        if(a[i][j]>255)
                            a[i][j]=255;
                    }
                b[ind]=a[i][j]/270.;

            }
    }

}
