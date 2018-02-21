#ifndef SERIALQOBJ_H
#define SERIALQOBJ_H
#include "headers.h"
//#include "ftt.h"
#include "stand_dev.h"
#include "serial.h"
#include "drawing.h"
#include "perceptron.h"
class serial_obj : public QObject
{
    Q_OBJECT
public:
//    Wavelet WT;
    int cnt;
    Serial hSerial;
    int* ind_c;
    int8_t EMG_I;
//    myCurve* MC;
//    standartDev STD[2];
//    frBuHp2 FBH[2];
//    //bandPassFr BPF[2];
//    matchedFr MF[4];
//    lowPassFr LPF[4];
//    lowPassFr2 LPF2[4];
//    matchedFrV MFV[2];
//    integrator INTEGR[2];
//    featureExtr1 FE1[2];
//    WillisonAmp WA[2];

    uint8_t readVar;
//    vector<fcomplex>& ft;
//    void work();
    int ptr;

    serial_obj(QString);
    void init(QString);
    ~serial_obj();
    void close();
public slots:
    vector<float> doWork();
signals:
    void dataOut(vector<float>);
};
#endif // SERIALQOBJ_H
