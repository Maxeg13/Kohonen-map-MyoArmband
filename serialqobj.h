#ifndef SERIALQOBJ_H
#define SERIALQOBJ_H
#include "headers.h"
#include "stand_dev.h"

class serial_obj : public QObject
{
    Q_OBJECT
public:
    standartDev STD[2];
    frBuHp2 FBH[2];
    //bandPassFr BPF[2];
    matchedFr MF[4];
    lowPassFr LPF[4];
    lowPassFr2 LPF2[4];
    matchedFrV MFV[2];
    integrator INTEGR[2];
    featureExtr1 FE1[2];
    WillisonAmp WA[2];
    int readVar;
    int gottenVar[2];
    QVector<float> featureOut;
    int ptr;
    serial_obj(QString);
    void init(QString);
    ~serial_obj();
    void close();
public slots:
    void doWork();
};
#endif // SERIALQOBJ_H
