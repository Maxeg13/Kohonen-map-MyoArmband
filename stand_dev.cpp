#include "include/stand_dev.h"  
#include<math.h>

int threshB(int x, int a)
{
    if(fabs(x)<(a))
        return(0);
    else
        return(1);
}

float upperVal(float x)
{
    if(x>0)return x;
    return 0;
}

int killRange(int x, int thr )
{
    if (x > thr)
        return (x - thr);
    if (x < -thr)
        return (x + thr);
    return (0);
}


int8_t thresh1(int x,int a,int b)
{
    if(x<a)return(a);
    if(x>b)return(b);
    return(x);
}



standartDev::standartDev()
{
    N=250;
    accumD=0;
    xPr=new int8_t[N];
    for(int i=0;i<N;i++)
        xPr[i]=0;
}

float standartDev::operator()(int y)
{
    j++;
    if(j==N)j=0;
    xPr[j]=threshB(y,3);
    accumD+=((int16_t)xPr[j])*((int16_t)xPr[j]);//int16_t
    accumD-=((int16_t)xPr[(j==(N-1))?0:(j+1)])*
            ((int16_t)xPr[(j==(N-1))?0:(j+1)]);

    return(200*sqrt((float)accumD/N));

}


float matchedFr::operator()(float xi)
{
    sum = 0;
    for (int8_t i = 0; i < MFN; i++)
    {
        sum += a[MFN - 1 - i] * x[i];
    }
    for (int8_t i = MFN - 1; i > 0; i--)
    {
        x[i] = x[i - 1];
    }
    x[0] = xi;
    return (sum);
}


matchedFr::matchedFr(){
    weight=7;
    MFN=7;
    a=new float[MFN];
    x=new float[MFN];

    a[0]=weight;
    a[1]=weight;
    a[2]=weight;
    a[3]=0;
    a[4]=-7;
    a[5]=-7;
    a[6]=-7;
}


long integrator::operator()(int xi)
{
    cnt++;
    buf += xi;
    return buf;
}



WillisonAmp::WillisonAmp()
{
    j=0;
    N=200;
    xPr=new int8_t[N];
    for (int i=0;i<N;i++)
        xPr[i]=0;
}


float WillisonAmp::operator()(int y)
{
    j++;
    if(j==N)j=0;
    xPr[j]=y;
    accumD+=threshB(((int)xPr[j])-((int)xPr[(j-1+N)%N]),3);//int16_t
    accumD-=threshB((int)xPr[(j+2)%N]-(int)xPr[(j+1)%N],3);
    return(2000*((float)accumD)/N);
}


float featureExtr1::operator()(int8_t x)
{

//            return(LPF(STD(x)));
//    return(LPF(IR(killRange(MF(x),30))));//demo of bipolar INTEGR
//        return(400*LPF2((killRange(MF(x),30))));//demo of bipolar LPF
//    return(LPF(WA(x)));
//    return(threshB(abs(x),20)*100);
    return(400*LPF2(upperVal(killRange(MF(x),30))));
}


float lowPassFr2::operator()(float x) //0.4 Hz
{
    v[0] = v[1];
            v[1] = v[2];
            v[2] = (1.576334488051500671e-6 * x)
                 + (-0.99645200271929113001 * v[0])
                 + (1.99644569738133892400 * v[1]);
            return
                 (v[0] + v[2])
                +2 * v[1];
}


float lowPassFr::operator()(float x) //class II
{
    v[0] = v[1];
    v[1] = v[2];
    v[2] = (6.300290918370700345e-5 * x)
            + (-0.97269313627177789172 * v[0])
            + (1.97244112463504306376 * v[1]);
    return
            (v[0] + v[2])
            +2 * v[1];
}
