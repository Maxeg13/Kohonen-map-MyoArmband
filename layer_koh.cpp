//dont forget rst
#include "layer_koh.h"
#include "headers.h"
using namespace std;
int ind_cen;
vector<vector<float>> dist2;

//#define TOROID
//vector<float> nullVect_m;
// EMG классификация позы при стрельбе из спортивного лука
float min(float x, float y)
{
    if(x>y)
        return y;
    else
        return x;
}

float dist2_F(QPoint a, QPoint b, int c, int d)
{
    return QPoint::dotProduct(a-(b+QPoint(c,d)),a-(b+QPoint(c,d)));
}

sector::sector()
{}
sector::sector(const QVector<QPoint> &QPT):QPolygon(QPT)
{}

void sector::rst()
{
    float range=500;
    float accum=100000000000;
//        while(accum>(range*range/4))
//        {
//            accum=0;
//            for(int i=0;i<size_in;i++)
//            {
//                //        w[i]=((rand()%10)/10.-0.5)*1;//2000

//                w[i]=((rand()%1000)/1000.)*range-range/2;
//                accum+=w[i]*w[i];
//            }
//        }

    accum=100000000000;
    while(accum>(range*range/4))
    {
        accum=0;
        for(int i=0;i<size_out;i++)
        {

            w_mot[i]=((rand()%1000)/1000.)*range-range/2;
            accum+=w_mot[i]*w_mot[i];
        }
    }


//    qDebug()<<ind_cen;
    float gauss;
    for(int i=0;i<size_in;i++)
    {
//        gauss=(rand()%10+rand()%10+rand()%10+rand()%10+rand()%10-25);
//        w[i]=gauss*exp(-dist2[ind_cen][ind]/120000000);
        w[i]=rand()%1000-500;

    }
}

sector::sector(vector<float>& inp, const QVector<QPoint> &QPT, QPoint c, int _ind):
    QPolygon(QPT),centre(c)
{
    ind=_ind;
    size_out=3;
    size_in=inp.size();
    w=new float[size_in];
    w_mot=new float[size_out];
    //    rst();
}


//sector::sector(int size_inp,const QVector<QPoint> &QPT,QPoint c):
//    QPolygon(QPT),centre(c)
//{
//    size_in=inp.size();
//    w=new float[size_in];
//    rst();
//}


float sector::getState(vector<float>& inp)
{
    state=0;
    for(int i=0;i<size_in;i++)
        state+=w[i]*inp[i];
}


float sector::getDiff(vector<float>& inp)
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
    t_mot=0;
}


layer_koh::layer_koh(vector<float>& inp_m,int N_m)
{
    Nx=N_m/3;
    width=(300+gap)*Nx;
    Ny=N_m;
    height=(86+gap*.36)*Ny;
    N=Nx*Ny;

    for(int k1=0;k1<Ny;k1++)
    {
        for(int i1=0;i1<Nx;i1++)
        {
            int i=(i1)%Nx;
            int k=(k1)%Ny;

            if(i1==(Nx/2-1))
                if(k1==(Ny/2))
                    ind_cen=k1*Nx+i1;
        }
    }

  qDebug()<<"still works";
    t=0;
    t_mot=0;
    s=0.7;
    gap=30;
    QPT_origin.reserve(6);
    QPT.reserve(6);
    is=0;
    ks=0;







    dist2.resize(N);
    for(int i=0;i<N;i++)
        dist2[i].resize(N);


    state=new float*[inp_m.size()];
    for(int i=0;i<inp_m.size();i++)
        state[i]=&SR[i].state;

    //    out_s=3;
    inp_s=inp_m.size();
    w=new float**[inp_m.size()];
    for(int i=0;i<inp_m.size();i++)
        w[i]=new float*[N];

    SR.reserve(N);


    x0=400;y0=100;
    QPT_origin.push_back(QPoint(-100,0));
    QPT_origin.push_back(QPoint(-50,86) );
    QPT_origin.push_back(QPoint(50,86) );
    QPT_origin.push_back(QPoint(100,0) );
    QPT_origin.push_back(QPoint(50,-86) );
    QPT_origin.push_back(QPoint(-50,-86) );
    QPT=QPT_origin;

    SR.resize(N);
    for(int k1=0;k1<Ny;k1++)
    {
        for(int i1=0;i1<Nx;i1++)
        {
            int i=(i1)%Nx;
            int k=(k1)%Ny;
            SHIFT=QPoint((300+gap)*i+x0-(150+gap/2)*(k%2),y0+(86+gap*.36)*k);
            for(int j=0;j<QPT_origin.size();j++)
                QPT[j]=QPT_origin[j]*s+SHIFT;
            SR[k1*Nx+i1]=(sector(inp_m,QPT,SHIFT,k1*Nx+i1));


            //            SR.
        }
    }



    //my fit
    ///////
    //    for(int i=0;i<N/2;i++)
    //    {
    //        SR[i].clr[0]=255;
    //        SR[i].clr[1]=0;
    //        SR[i].clr[2]=0;
    //    }


    for(int i=(0);i<N;i++)
    {
        SR[i].clr[0]=0;
        SR[i].clr[1]=0;
        SR[i].clr[2]=255;
    }


    out=new float*[N];
    for(int i=0;i<N;i++)
        out[i]=&SR[i].diff;



//    for(int i=0;i<N;i++)
//        for(int j=0;j<N;j++)
//        {
//            qDebug()<<i;

//            dist2[i][j];
//        }


    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
        {
#ifdef TOROID
            dist2[i][j]=min(dist2_F(SR[i].centre, SR[j].centre,0,0),dist2_F(SR[i].centre, SR[j].centre,width,0));
            dist2[i][j]=min(dist2[i][j],dist2_F(SR[i].centre, SR[j].centre,-width,0));
            dist2[i][j]=min(dist2[i][j],dist2_F(SR[i].centre, SR[j].centre,0,height));
            dist2[i][j]=min(dist2[i][j],dist2_F(SR[i].centre, SR[j].centre,0,-height));
            dist2[i][j]=min(dist2[i][j],dist2_F(SR[i].centre, SR[j].centre,-width,-height));
            dist2[i][j]=min(dist2[i][j],dist2_F(SR[i].centre, SR[j].centre,width,-height));
            dist2[i][j]=min(dist2[i][j],dist2_F(SR[i].centre, SR[j].centre,-width,height));
            dist2[i][j]=min(dist2[i][j],dist2_F(SR[i].centre, SR[j].centre, width,height));


#else
            dist2[i][j]=(dist2_F(SR[i].centre, SR[j].centre,0,0));
#endif
        }

    for(int i=0;i<N;i++)
        SR[i].rst();

    for(int i=0;i<inp_m.size();i++)
        for(int j=0;j<N;j++)
            w[i][j]=&SR[j].w[i];

}

void layer_koh::reform()
{
    //    qDebug()<<is;
    int ind_h;
    for(int k1=0;k1<Ny;k1++)
    {
        for(int i1=0;i1<Nx;i1++)
        {
            int i=(i1+is+Nx)%Nx;
            int k=(k1+ks+Ny)%Ny;
            ind_h=k1*Nx+i1;
            SHIFT=QPoint((300+gap)*i+x0-(150+gap/2)*(k%2),y0+(86+gap*.36)*k);
            for(int j=0;j<QPT_origin.size();j++)
                if(ind_h!=ind)
                    SR[ind_h][j]=QPT_origin[j]*s*SR[ind_h].diff_norm_inv*SR[ind_h].diff_norm_inv*SR[ind_h].diff_norm_inv+SHIFT;
                else
                    SR[ind_h][j]=QPT_origin[j]*s+SHIFT;
        }
    }
}

void layer_koh::GET_TOROID_SHIFT(int _is, int _ks)
{
    is=_is;
    ks=_ks;
}

int layer_koh::indOfMin(const vector<float>& _inp)
{
    inp=_inp;
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

void layer_koh::learnW(const vector<float>& inp,float rad)
{
    t++;
    int ind=0;
    float h1;
    speed_k=0.0016;//0.0016

    ind=indOfMin(inp);
    float exp_val=exp(-0.0005*t);
    float speed_exp=exp(-0.0004*t);
    for(int i=0;i<N;i++)
    {
        h1=dist2[i][ind];

        //        float h_func=exp(-h1/(6400000*rad*exp_val*exp_val+0.00001));//.0000001
        float h_func=exp(-h1/(3000000*rad*exp_val+0.00001));//.0000001
        /////////////////////////////2400000
        for(int j=0;j<SR[i].size_in;j++)
        {
            SR[i].w[j]+=speed_k*speed_exp*h_func*
                    (inp[j]-SR[i].w[j]);
        }
    }
}

void layer_koh::learnW_mot( vector<float>& inp,  vector<float>& out,float rad)
{
    t_mot++;
    int ind=0;
    float h1;
    speed_k=0.0016;

    ind=indOfMin(inp);
    float exp_val=exp(-0.0005*t_mot);
    float speed_exp=exp(-0.00006*t);
    for(int i=0;i<N;i++)
    {
        h1=dist2[i][ind];

        //        float h_func=exp(-h1/(6400000*rad*exp_val*exp_val+0.00001));//.0000001
        float h_func=exp(-h1/(3000000*rad*exp_val+0.00001));//.0000001
        //////////////////////////////2400000
        for(int j=0;j<SR[i].size_out;j++)
        {
            SR[i].w_mot[j]+=speed_k*speed_exp*h_func*
                    (out[j]-SR[i].w_mot[j]);
        }
    }
}

float** layer_koh::refresh(vector<float>& inp, int& _ind)
{
    ind=indOfMin(inp);
    _ind=ind;
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
        SR[k].diff_norm=(SR[k].diff-diff_min)*diff_k;
        SR[k].diff_norm_inv=1-SR[k].diff_norm;
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
            painter.fillPath(path,QBrush(QColor(255*SR[i].diff_norm,255*SR[i].diff_norm,
                                                255,150)));//150
        else
            painter.fillPath(path,QBrush(QColor(255,255,255)));

        painter.drawPolygon(SR[i]);
    }
}


