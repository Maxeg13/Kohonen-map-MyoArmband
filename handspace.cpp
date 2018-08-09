#include "handspace.h"
#include <QDebug>
#include <QMouseEvent>
using namespace std;
bool pressed;
HandSpace::HandSpace(QWidget *parent) : QWidget(parent)
{

}


void HandSpace::mousePressEvent(QMouseEvent *e)
{
    pressed=1;
    QPointF p=e->pos()/2.5+QPointF(-180,0);
    qDebug()<<p;
    vector<int> angles;
    angles.push_back(p->x());
    angles.push_back(p->y());
    angles.push_back(0);
    emit sendAngles(angles);
}

void HandSpace::mouseReleaseEvent(QMouseEvent *e)
{
    pressed=0;
}

void HandSpace::mouseMoveEvent(QMouseEvent *e)
{
    if(pressed)
    {
        QPointF p=e->pos()/2.5+QPointF(-180,0);
        qDebug()<<p;
        vector<int> angles;
        angles.push_back(p->x());
        angles.push_back(p->y());
        angles.push_back(0);
        emit sendAngles(angles);
    }
}
