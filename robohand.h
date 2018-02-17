#ifndef ROBOHAND_H
#define ROBOHAND_H
#include <QPoint>

class Robohand
{
public:

    float data[3];
    Robohand();
    QPoint stat_origin[3],origin[3];
    QPoint stat_bones[3][4], bones[3][4];
    float length;
    float width;
    void get_bones();
private:
    float k=3.14/180;
};

#endif // ROBOHAND_H
