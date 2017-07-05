#ifndef LAYER_KOH_H
#define LAYER_KOH_H
#include "headers.h"
//QVector<float> nullVect_m;
// EMG классификация позы при стрельбе из спортивного лука

class sector:public QPolygon
{
public:
    float* w;
    float diff_norm, diff_norm_inv;
    int size_in;
    void rst();
    QPoint centre;
    float state;
    float diff;
    sector();
    sector(const QVector<QPoint>&);
    sector(QVector<float>& ,const QVector<QPoint>&,QPoint );
    float getState(QVector<float>& );
    float getDiff(QVector<float>& );
};

class layer_koh
{
public:
    float thresh(float , int ,int );
    int ind;
    int rad;
    float t;
    float** out;
    float speed_k;
    float diff_min, diff_max, diff_k;

    int N,Nx,Ny;
    //    QVector<float>& out;
    QPoint SHIFT;
    QVector<QPoint> QPT_origin, QPT;
    float x0,y0,s,gap;
    QVector<sector> SR;
    QVector<QVector<float>> dist2;

    void reform();
    layer_koh(QVector<float>& inp_m,int N_m);

    int indOfMin(const QVector<float>& inp);
    void learnBegin();
    void learnW(const QVector<float>& inp);
    float** refresh(QVector<float>& inp);
    void draw(QPainter& painter);
    void rst();
};




#endif // LAYER_KOH_H
