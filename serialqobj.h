#ifndef SERIALQOBJ_H
#define SERIALQOBJ_H
#include "headers.h"
class serial_obj : public QObject
{
    Q_OBJECT
public:
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
