#include "handspace.h"
#include <QDebug>
#include <QMouseEvent>
using namespace std;
bool pressed;
int wheel_angle;
vector<int> angles;

HandSpace::HandSpace(QWidget *parent) : QWidget(parent)
{
    angles.resize(3);
    angles[1]=180;
    emit sendAngles(angles);
}


void HandSpace::mousePressEvent(QMouseEvent *e)
{
    pressed=1;
    QPointF p=e->pos()/1.6+QPointF(-180,0);
//    qDebug()<<p;

    angles[0]=(p.x());
    angles[1]=(p.y());
    angles[2]=(wheel_angle);
    emit sendAngles(angles);
}

void HandSpace::mouseReleaseEvent(QMouseEvent *e)
{
    pressed=0;
}

void HandSpace::wheelEvent(QWheelEvent *e)
{

    int range=50;
    wheel_angle+=e->delta()/8;
    if(wheel_angle<-0)
        wheel_angle=-0;
    else if(wheel_angle>range)
        wheel_angle=range;
    emit sendAngles(angles);

    angles[2]=(wheel_angle);
    qDebug()<<wheel_angle;
}

void HandSpace::mouseMoveEvent(QMouseEvent *e)
{
    if(pressed)
    {
        QPointF p=e->pos()/1.6+QPointF(-180,0);
//        qDebug()<<p;
//        vector<int> angles;
        angles[0]=(p.x());
        angles[1]=(p.y());
        angles[2]=(wheel_angle);
        emit sendAngles(angles);
    }
}
