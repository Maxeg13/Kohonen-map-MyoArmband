#ifndef LAYER_KOH_H
#define LAYER_KOH_H
#include "headers.h"
using namespace std;
//vector<float> nullVect_m;
// EMG классификация позы при стрельбе из спортивного лука

class sector:public QPolygon
{
public:
    int clr[3];
    float* w;
    float* w_mot;
    float diff_norm, diff_norm_inv;
    int size_out;
    int size_in;
    void rst();
    QPoint centre;
    float state;
    float diff;
    sector();
    sector(const QVector<QPoint>&);
//    sector(int ,const QVector<QPoint>&,QPoint );
    sector(vector<float>& ,const QVector<QPoint>&,QPoint );
    float getState(vector<float>& );
    float getDiff(vector<float>& );
};

class layer_koh
{
public:
    int is, ks;
    int inp_s;
    vector<float> inp;
    float** state;
    float thresh(float , int ,int );
    int ind;
    int rad;
    float t;
    float t_mot;
    float** out;
    float speed_k;
    float diff_min, diff_max, diff_k;
    float*** w;
    int N,Nx,Ny;
    int width;
    int height;
    //    vector<float>& out;
    QPoint SHIFT;
    QVector<QPoint> QPT_origin, QPT;
    float x0,y0,s,gap;
    QVector<sector> SR;
    vector<vector<float>> dist2;

    void reform();
    layer_koh(vector<float>& inp_m,int N_m);
    void GET_TOROID_SHIFT(int _is, int _ks);
    int indOfMin(const vector<float>& inp);
    void learnBegin();
    void learnW(const vector<float>& inp,float rad);
    void learnW_mot( vector<float>& inp,  vector<int> &out, float rad);
    float** refresh(vector<float>& inp, int&);
    void draw(QPainter& painter);
    void rst();
};




#endif // LAYER_KOH_H
