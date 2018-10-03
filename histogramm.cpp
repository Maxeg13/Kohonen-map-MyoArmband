#include "histogramm.h"

Histogramm::Histogramm()
{
    N=8;
    N2=N*N;
    k=0.9985;
    width=20;
    range=170;
    discr=range/N;
    incr=3;

    b_ind=new int*[N];
    for(int i=0;i<N;i++)
        b_ind[i]=new int[N];

    grid_out=new int [N+1];

    grid=new int [N+1];

    for(int i=0;i<(N+1);i++)
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
    int i0=0,j0=0;
    int ind=0;
    int isHitY=0;
    int isHitX=0;
    {
        for(int i=0;i<(N);i++)
            if((y>=grid[i])&(y<=grid[i+1]))
            {
                i0=i;
                isHitY=1;
            }

        for(int j=0;j<(N);j++)
            if((x>=grid[j])&(x<=grid[j+1]))
            {
                j0=j;
                isHitX=1;
            }


        if(!isHitX)
        {
            if(x<grid[0])
                j0=0;
            else if(x>grid[N])
                j0=N-1;
        }

        if(!isHitY)
        {
            if(y<grid[0])
                i0=0;
            else if(y>grid[N])
                i0=N-1;
        }

        a[i0][j0]+=incr;
        if(a[i0][j0]>255)
            a[i0][j0]=255;

        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
            {

                a[i][j]*=k;

//                if((i==3)||(i==4))
//                    if((j==3)||(j==4))
//                        a[i][j]=0;

                b[ind]=a[i][j]/270.;
                ind++;

            }

    }

}
