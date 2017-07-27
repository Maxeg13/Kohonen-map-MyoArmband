#ifndef LAYER_KOH_H
#define LAYER_KOH_H
#include "headers.h"

//std::vector<float> nullVect_m;
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
    sector(std::vector<float>& ,const QVector<QPoint>&,QPoint );
    float getState(std::vector<float>& );
    float getDiff(std::vector<float>& );
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
    //    std::vector<float>& out;
    QPoint SHIFT;
    QVector<QPoint> QPT_origin, QPT;
    float x0,y0,s,gap;
    QVector<sector> SR;
    std::vector<std::vector<float>> dist2;

    void reform();
    layer_koh(std::vector<float>& inp_m,int N_m);

    int indOfMin(const std::vector<float>& inp);
    void learnBegin();
    void learnW(const std::vector<float>& inp,float rad);
    float** refresh(std::vector<float>& inp);
    void draw(QPainter& painter);
    void rst();
};




#endif // LAYER_KOH_H
