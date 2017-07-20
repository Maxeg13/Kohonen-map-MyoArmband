#include "layer_koh.h"
#include "headers.h"
//QVector<float> nullVect_m;
// EMG классификация позы при стрельбе из спортивного лука

sector::sector()
{}
sector::sector(const QVector<QPoint> &QPT):QPolygon(QPT)
{}

void sector::rst()
{
    for(int i=0;i<size_in;i++)
        w[i]=((rand()%10)/10.-0.5)*1;//2000
}

sector::sector(QVector<float>& inp,const QVector<QPoint> &QPT,QPoint c):
    QPolygon(QPT),centre(c)
{
    size_in=inp.size();
    w=new float[size_in];
    rst();
}


float sector::getState(QVector<float>& inp)
{
    state=0;
    for(int i=0;i<size_in;i++)
        state+=w[i]*inp[i];
}


float sector::getDiff(QVector<float>& inp)
{
    diff=0;
    float h;
    for(int i=0;i<size_in;i++)
    {
        h=w[i]-inp[i];
        diff+=h*h;
    }
    diff=sqrt(diff);
}


float layer_koh::thresh(float x, int x1,int x2)
{
    if(x>x2)
        return x2;
    if(x<x1)
        return(x1);
    return x;
}

void layer_koh::rst()
{
    for (int i=0;i<N;i++)
        SR[i].rst();
    t=0;
}

void layer_koh::reform()
{
    int ind_h;
    for(int k=0;k<Ny;k++)
    {
        for(int i=0;i<Nx;i++)
        {
            ind_h=k*Nx+i;
            SHIFT=QPoint((300+gap)*i+x0-(150+gap/2)*(k%2),y0+(86+gap*.36)*k);
            for(int j=0;j<QPT_origin.size();j++)
                if(ind_h!=ind)
                    SR[ind_h][j]=QPT_origin[j]*s*SR[ind_h].diff_norm*SR[ind_h].diff_norm*SR[ind_h].diff_norm+SHIFT;
                else
                    SR[ind_h][j]=QPT_origin[j]*s+SHIFT;
        }
    }
}
layer_koh::layer_koh(QVector<float>& inp_m,int N_m)
{
    t=0;
        s=0.7,gap=30;
    QPT_origin.reserve(6);
    QPT.reserve(6);

    Nx=N_m/3;
    Ny=N_m;
    N=Nx*Ny;

    SR.reserve(N);
    dist2.resize(N);
    for(int i=0;i<N;i++)
        dist2[i].resize(N);

    x0=400;y0=100;
    QPT_origin.push_back(QPoint(-100,0));
    QPT_origin.push_back(QPoint(-50,86) );
    QPT_origin.push_back(QPoint(50,86) );
    QPT_origin.push_back(QPoint(100,0) );
    QPT_origin.push_back(QPoint(50,-86) );
    QPT_origin.push_back(QPoint(-50,-86) );
    QPT=QPT_origin;

    for(int k=0;k<Ny;k++)
    {
        for(int i=0;i<Nx;i++)
        {
            SHIFT=QPoint((300+gap)*i+x0-(150+gap/2)*(k%2),y0+(86+gap*.36)*k);
            for(int j=0;j<QPT_origin.size();j++)
                QPT[j]=QPT_origin[j]*s+SHIFT;
            SR.push_back(sector(inp_m,QPT,SHIFT));
        }
    }
    out=new float*[N];
    for(int i=0;i<N;i++)
        out[i]=&SR[i].diff;


    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
        {
            dist2[i][j]=QPoint::dotProduct(SR[i].centre-
                                           SR[j].centre,
                                           SR[i].centre-
                                           SR[j].centre);
        }
}


int layer_koh::indOfMin(const QVector<float>& inp)
{
int ind_h;
    float h1;
    float sumMin=100000000000000;
    float accum;
    for(int i=0;i<N;i++)
    {
        accum=0;
        for(int j=0;j<SR[i].size_in;j++)
        {
            h1=(SR[i].w[j]-inp[j]);
            accum+=h1*h1;
        }
        if(accum<sumMin)
        {
            sumMin=accum;
            ind_h=i;
        }
    }
    return(ind_h);
}

void layer_koh::learnBegin()
{
    t=0;
}

void layer_koh::learnW(const QVector<float>& inp)
{
    t++;
    int ind=0;
    float h1;
    speed_k=0.004;


    ind=indOfMin(inp);

    for(int i=0;i<N;i++)
    {
        h1=dist2[i][ind];
        float exp_val=exp(-0.001*t);
        float h_func=exp(-h1/(2400000*exp_val*exp_val+0.00001));//.0000001

        for(int j=0;j<SR[i].size_in;j++)
        {
            SR[i].w[j]+=speed_k*h_func*
                    (inp[j]-SR[i].w[j]);
        }
    }
}
float** layer_koh::refresh(QVector<float>& inp)
{
    ind=indOfMin(inp);
    for(int k=0;k<N;k++)
        SR[k].getDiff(inp);

    diff_min=100000000000000;
    for(int i=0;i<N;i++)
        if(SR[i].diff<diff_min)
            diff_min=SR[i].diff;

    diff_max=0;
    for(int i=0;i<N;i++)
        if(diff_max<SR[i].diff)
            diff_max=SR[i].diff;

    diff_k=1./(diff_max-diff_min);

    for(int k=0;k<N;k++)
    {
        SR[k].diff_norm_inv=(SR[k].diff-diff_min)*diff_k;
        SR[k].diff_norm=1-SR[k].diff_norm_inv;
        SR[k].diff_norm_inv=sqrt(SR[k].diff_norm_inv);
    }
    return(out);
}
void layer_koh::draw(QPainter& painter)
{
    for(int i=0;i<SR.size();i++)
    {
        QPainterPath path;
        path.addPolygon(SR[i]);
        if(i!=ind)
            painter.fillPath(path,QBrush(QColor(255*SR[i].diff_norm_inv,255*SR[i].diff_norm_inv,
                                                255,150)));
        else
            painter.fillPath(path,QBrush(QColor(255,255,255)));

        painter.drawPolygon(SR[i]);
    }
}


