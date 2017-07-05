#ifndef MYOLIB_H
#define MYOLIB_H
#include <stdint.h>
int rejectThresh(int x,int a)
{
    if(x<-a)return(x+a);
    if(x>a)return(x-a);
    return(0);
}

int8_t thresh(int x,int a,int b)
{
    if(x<a)return(a);
    if(x>b)return(b);
    return(x);
}






//1000 46 48
class  rejectFr
{	
private:
    float v[3]={0,0,0};
public:
    float operator()(float x) //class II
    {
        v[0] = v[1];
        v[1] = v[2];
        v[2] = (9.874651320215761530e-1 * x)
                + (-0.97493026404315141775 * v[0])
                + (1.87647537020623267345 * v[1]);
        return
                (v[0] + v[2])
                - 1.900295 * v[1];
    }
};


class calibr
{
public:
    long timeM;
    double  maxM = 5, minM = -5, divM = 0;
    double minL = -125, maxL = 125;
    double a = maxL / maxM, b = minL/minM;


    int T1=1500,T2=3500;
    bool tr;

    void getM(int x , unsigned long y)
    {
        tr=((y<T2)&&(y>T1));
        if (tr)
        { divM++;
            if (divM == 3)
            {
                divM = 0;
                if ((x) > maxM)
                {
                    maxM = x;
                    a = maxL / maxM;
                }

                if (x < minM)
                {
                    minM = x;
                    b = minL/minM;
                }
            }
        }
    }

    int setM(double x)
    {
        if (x > 0)return (x * a);
        else return (x * b);
    }
};



int threshI(int x)
{
    if (x < -127)return (-127);
    if (x > 127) return 127;
    return x;
}

int PLUS(double x)
{
    if (x < 0)return (0);
    else return(x);

}









//decoder DECR(x, y, z);
//singleDec one;

class bandPassFr
{

public:

    float v[7]={0,0,0,0,0,0,0};

    float  operator()(float x) //class II
    {
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = (4.057585794688801947e-6 * x)
                + (-0.99310232791388763918 * v[0])
                + (3.97927634173555855313 * v[1])
                + (-5.97924564872607611221 * v[2])
                + (3.99307163484980209844 * v[3]);
        return
                (v[0] + v[4])
                - 2 * v[2];
    }
};




//High pass bessel filter order=2 alpha1=0.001
class  frBeHp1
{
public:
    float v[3]={0,0,0};

    float operator()(float x) //class II
    {
        v[0] = v[1];
        v[1] = v[2];
        v[2] = (1.308227021366165088e-2 * x)
                + (-0.97517787618064888377 * v[0])
                + (1.97498292688290821850 * v[1]);
        return
                (v[2] - v[0]);}
};




class  frBuHp2
{
public:

    float v[3]={0,0,0};

    float operator()(float x) //class II
    {
        v[0] = v[1];
        v[1] = v[2];
        v[2] = (9.385006906693222906e-1 * x)
                + (-0.88078712935881264823 * v[0])
                + (1.87321563331847573686 * v[1]);
        return
                (v[0] + v[2])
                - 2 * v[1];
    }
};


class slowFr
{
public:

    double v[3]={0,0,0};

    double operator()(double x) //class II
    {
        v[0] = v[1];
        v[1] = v[2];
        v[2] = (3.046271917353494518e-3 * x)
                + (-0.99436106191873174787 * v[0])
                + (1.99435712519859364988 * v[1]);
        return
                (v[2] - v[0]);
    }
};

//BeHp[0]=BeHp(1);










class lastStage   //abstract motor class
{
public:
    int16_t count;
    double tPar=150.;
    double k=1.2/tPar;
    double x;
    int realX;

    void setTarg(int e)
    {
        count++;
        if (count == tPar)
        {
            realX = e;
            count = 0;
        }
    }
    int operator()(int e)
    {
        setTarg(e);
        x += (realX - x) *k;
        return x;
    }
};




class median
{	
public:
    int m;
    int N=16;
    int N_x=N-2;
    int hN=N/2;
    int ind=0;
    int cnt=0;
    double x[16];
    double cst[16];

    double operator()(double in)
    {
        cnt++;
        if(cnt==4)
        {
            ind++;
            ind=(ind==N)?0:ind;
            cst[ind]=in;

            for(int i=0;i<N;i++)
                x[i]=cst[i];
            cnt=0;
            for	(int i=(N-1);i>3;i--)
                for (int j=1;j<=i;j++)
                {
                    if(x[j]>x[j-1])
                    {
                        m=x[j];
                        x[j]=x[j-1];
                        x[j-1]=m;
                    }
                }
        }
        return(x[N_x]);
    }
};


#endif
