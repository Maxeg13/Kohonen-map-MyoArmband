#ifndef LAYER_KOH_H
#define LAYER_KOH_H
#include "headers.h"

//std::vector<float> nullVect_m;
// EMG классификация позы при стрельбе из спортивного лука
class layer_koh;
class sector;

class sector:public QPolygon
{
public:
//    static layer_koh* LK;

    float* w;
    float diff_norm, diff_norm_inv;
    int size_in;
    void rst(int i, int k, layer_koh *LK);
    QPoint centre;
    float state;
    float diff;
    sector();
    sector(const QVector<QPoint>&);
//    sector(int ,const QVector<QPoint>&,QPoint );
    sector(std::vector<float>& , const QVector<QPoint>&, QPoint , int i, int k, layer_koh*);
    float getState(std::vector<float>& );
    float getDiff(std::vector<float>& );
};

class layer_koh
{
public:
    int is, ks;
    int inp_s;
    std::vector<float> inp;
    float** state;
    float thresh(float , int ,int );
    int ind;
    int rad;
    float t;
    float** out;
    float speed_k;
    float diff_min, diff_max, diff_k;
    float*** w;
    int N,Nx,Ny;
    int width;
    int height;
    //    std::vector<float>& out;
    QPoint SHIFT;
    QVector<QPoint> QPT_origin, QPT;
    float x0,y0,s,gap;
    QVector<sector> SR;
    std::vector<std::vector<float>> dist2;

    void reform();
    layer_koh(std::vector<float>& inp_m,int N_m);
    void GET_TOROID_SHIFT(int _is, int _ks);
    int indOfMin(const std::vector<float>& inp);
    void learnBegin();
    void learnW(const std::vector<float>& inp,float rad);
    float** refresh(std::vector<float>& inp);
    void draw(QPainter& painter);
    void rst();
};




#endif // LAYER_KOH_H
