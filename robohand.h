#ifndef ROBOHAND_H
#define ROBOHAND_H
#include <QPoint>

class Robohand
{
public:
    float min[3],max[3];
    float data[3];
    Robohand();
    QPoint origin[4];
    QPoint stat_bones[3][4], bones[3][4];
    float length;
    float width;
    void get_bones();
    void set_form(int i, float width_k, float length_k);
    float k;


};

#endif // ROBOHAND_H
